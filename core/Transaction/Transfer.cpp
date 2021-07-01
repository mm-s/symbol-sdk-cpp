/**
*** Copyright (c) 2016-2019, Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp.
*** Copyright (c) 2020-present, Jaguar0625, gimre, BloodyRookie.
*** All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/

#include "Transfer.h"
#include "../catapult/BufferInputStreamAdapter.h"
#include "../catapult/EntityIoUtils.h"
#include "../Network.h"
#include "../catapult/TransferBuilder.h"
#include "../catapult/SharedKey.h"
#include "../catapult/CryptoUtils.h"
#include "../catapult/AesDecrypt.h"
#include "../catapult/SecureRandomGenerator.h"
#include "../catapult/OpensslContexts.h"
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <openssl/evp.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif


namespace symbol { namespace core {
	/// Implementation for the class c 
	using c = core::Transfer;
	using std::move;
	using std::ostringstream;
	using std::make_pair;

	c::Transfer(const Network& n, ptr<catapult::model::TransferTransaction> t): m_catapultTransferTx(t), b(n, t) {
	}

	c::Transfer(Transfer&& other): b(move(other)), m_catapultTransferTx(other.m_catapultTransferTx) {
		other.m_catapultTransferTx = nullptr;
	}

	pair<ko, ptr<Transfer>> c::create(const Network& n, const Blob& mem) {
		auto is = catapult::io::BufferInputStreamAdapter(mem);

	//	catapult::model::IsSizeValidT<catapult::model::Transaction>
	//	EntityType type=
	//	BasicEntityType bet=catapult::model::ToBasicEntityType(entity.Type);
		std::unique_ptr<catapult::model::TransferTransaction> ptx;
		try {
			ptx = catapult::io::ReadEntity<catapult::model::TransferTransaction>(is);
			assert(ptx->Type == catapult::model::Entity_Type_Transfer);
		}
		catch (...) {
			///CATAPULT_THROW_INVALID_ARGUMENT("size is insufficient");
			return make_pair("KO 84039", nullptr);
		}
		if (n.identifier() != ptx->Network) {
			return make_pair("KO 82291", nullptr);
		}
		return make_pair(ok, new Transfer(n, ptx.release()));
	}

/*
        uint64_t Random() {
                return utils::LowEntropyRandomGenerator()();
        }

        uint8_t RandomByte() {
                return static_cast<uint8_t>(Random());
        }


std::generate_n(container.begin(), container.size(), []() {
                                return static_cast<typename T::value_type>(RandomByte());
                        });
*/

	namespace {
		using namespace catapult;

		void Prepend(std::vector<uint8_t>& buffer, RawBuffer prefix) {
				buffer.resize(buffer.size() + prefix.Size);
				std::memmove(buffer.data() + prefix.Size, buffer.data(), buffer.size() - prefix.Size);
				std::memcpy(buffer.data(), prefix.pData, prefix.Size);
		}

		void AesGcmEncrypt(
						const crypto::SharedKey& encryptionKey,
						const crypto::AesGcm256::IV& iv,
						const RawBuffer& input,
						std::vector<uint8_t>& output) {
			// encrypt input into output
			output.resize(input.Size);
			auto outputSize = static_cast<int>(output.size());
			utils::memcpy_cond(output.data(), input.pData, input.Size);

			crypto::OpensslCipherContext cipherContext;
			cipherContext.dispatch(EVP_EncryptInit_ex, EVP_aes_256_gcm(), nullptr, encryptionKey.data(), iv.data());

			if (0 != outputSize)
					cipherContext.dispatch(EVP_EncryptUpdate, output.data(), &outputSize, output.data(), outputSize);

			cipherContext.dispatch(EVP_EncryptFinal_ex, output.data() + outputSize, &outputSize);

			// get tag
			crypto::AesGcm256::Tag tag;
			cipherContext.dispatch(EVP_CIPHER_CTX_ctrl, EVP_CTRL_GCM_GET_TAG, 16, tag.data());

			// tag || iv || data
			Prepend(output, iv);
			Prepend(output, tag);
		}
	}


	pair<ko, c::Msg> c::encrypt(const c::Msg& clearText, const PrivateKey& src, const PublicKey& rcpt) {
		using namespace catapult::crypto;
		Keys srcKeys(src); 
		auto sharedKey = DeriveSharedKey(srcKeys, rcpt);
		
		Msg encrypted;
		SecureRandomGenerator g;
		AesGcm256::IV iv;
		try {
			g.fill(iv.data(), iv.size());
		}
		catch(...) {
			return make_pair("KO 88119 Could not generate secure random bits.", move(encrypted));
		}
		AesGcmEncrypt(sharedKey, iv, clearText, encrypted);
		return make_pair(ok, move(encrypted));
	}

	pair<ko, ptr<Transfer>> c::create(const Network& n, const UnresolvedAddress& rcpt, const MosaicValues& m, const Amount& maxfee, const TimeSpan& deadline, const Msg& msg, const ptr<PrivateKey>& encryptPrivateKey, const ptr<PublicKey>& encryptPublicKey) {
		PublicKey unused;
		catapult::builders::TransferBuilder builder(n.identifier(), unused);

		Msg finalMsg;
		if (encryptPrivateKey != nullptr) {
			if (encryptPublicKey == nullptr) {
				return make_pair("KO 33092 Recipient Public Key is required.", nullptr); 
			}
			if (msg.empty()) {
				return make_pair("KO 33092 Message is empty.", nullptr); 
			}
			auto addr = n.newAddress(*encryptPublicKey);
			if (*addr != rcpt) {
				delete addr;
				return make_pair("KO 33092 Encryption Public Key must correspond to the recipient address.", nullptr); 
			}
			delete addr;
			auto r = encrypt(msg, *encryptPrivateKey, *encryptPublicKey);
			if (is_ko(r.first)) {
				return make_pair(r.first, nullptr); 
			}
			Keys myKeys(*encryptPrivateKey);
			finalMsg.resize(1+myKeys.publicKey().Size+r.second.size());
			finalMsg[0]='\1';
			auto dest=finalMsg.data()+1;
			memcpy(dest, myKeys.publicKey().data(), myKeys.publicKey().Size);
			dest+=myKeys.publicKey().Size;
			memcpy(dest, r.second.data(), r.second.size());
		}
		else {
			finalMsg=msg;
			finalMsg.insert(finalMsg.begin(), '\0');
		}
		if (!finalMsg.empty()) {
			builder.setMessage(catapult::utils::RawBuffer(finalMsg.data(), finalMsg.size()));
		}
		builder.setRecipientAddress(rcpt);
	//	for (const auto& seed : seeds) {
	//		auto mosaicId = mosaicNameToMosaicIdMap.at(seed.Name);
	//		builder.addMosaic({ mosaicId, seed.Amount });
	//	}
//cout << "xXXXXXXXXXXXXXX " << m.unwrap() << endl;
		//UnresolvedMosaicId um(m.unwrap());
		for (auto& i: m) {
			builder.addMosaic({ UnresolvedMosaicId{i.first.unwrap()}, i.second });
		}
		builder.setDeadline(Timestamp(deadline.millis()));
		auto x = builder.build().release();
//cout << "XXXXXXXXXXXXXXXXXXXX" << endl;
//cout << x->Network << " " << x->Size << " bytes" << endl;
		
		return make_pair(ok, new Transfer(n, x));
	}

	bool c::toStream(ostream& os) const {
		if (m_catapultTx==nullptr) {
			return false;
		}
		Blob buf;
		buf.resize(m_catapultTx->Size);
		memcpy(buf.data(), m_catapultTx, m_catapultTx->Size);
		os << catapult::utils::HexFormat(buf);
		return true;
	}

}} // namespaces

std::ostream& operator << (std::ostream& os, const symbol::core::Transfer& o) {
	o.toStream(os);
	return os;
}

