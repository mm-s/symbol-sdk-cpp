#include <symbol/symbol.h>
#include <symbol/core.h>
#include <iostream>
#include <cassert>
//#include "catapult/TestHarness.h"

using namespace std;
using namespace symbol;

using ko=symbol::ko;


void test_core_hmi_stack() {
	/// class at top (of the inheritance stack) (hmi/Hmi.h) shall be: Wallet
/*
	struct {
		
	};

Wallet
  NonFungibleToken
  DigitalAssets
Transaction
Keys
Main

*/
}

void test_core_hmi() {
	test_core_hmi_stack();
}


/*
 void shouldCreateAccountViaStaticConstructor() { //https://github.com/nemtech/symbol-sdk-java/blob/main/sdk-core/src/test/java/io/nem/symbol/sdk/model/account/AccountTest.java
    Account account = Account.createFromPrivateKey("787225AAFF3D2C71F4FFA32D4F19EC4922F3CD869747F267378F81F8E3FCB12D", NetworkType.MIJIN_TEST);
    assertEquals("787225AAFF3D2C71F4FFA32D4F19EC4922F3CD869747F267378F81F8E3FCB12D", account.getPrivateKey());
    assertEquals("2134E47AEE6F2392A5B3D1238CD7714EABEB739361B7CCF24BAE127F10DF17F2", account.getPublicKey());
    assertEquals("SAEJCCEGA5SMEL65GTVYS6P6V2F5TOPDAOVAC5Q", account.getAddress().plain());
  }
*/

void test_keys() {
//	auto k=Keys::generate();
//	assert(k.first==ok);
//	cout << k.second << endl;
}

void test_net() {
/*
	mainnet mn;
	cout << "mainnet:" << mn << endl;
	testnet tn;
	cout << "testnet:" << tn << endl;
	auto k=keys::generate();
	assert(k.first==ok);
	cout << k.first << endl;
	auto& mnaddr=*mn.new_address(k.second);
	auto& tnaddr=*tn.new_address(k.second);
	cout << "addr (mainnet): " << mnaddr << " valid: " << mnaddr.is_valid() << endl;
	cout << "addr (testnet): " << tnaddr << " valid: " << tnaddr.is_valid() << endl;
	delete &mnaddr;
	delete &tnaddr;
*/
}

void test_mosaic() {
}


void cli_account_generate() {
}

void cli_account_info() {
}

void cli_account() {
	cli_account_generate();
	cli_account_info();
}

void cli() {
	cli_account();
}

void test_core() {
	test_keys();
	//tx();
	test_net();
	test_mosaic();
	test_core_hmi();

	cli();
	//model::PublicKeyToAddress(publicKey, model::NetworkIdentifier::Zero)

	/// Aditional tests:
	/// bin/e2e
}

void test_rpc() {
}

//int main() {
//	test_core();
//	test_rpc();
//	return 0;
//}
#include <gtest/gtest.h>
#include <symbol/core.h>
#include <tuple>
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
using std::tuple;

namespace {
	tuple<int, string, string> exec_offline(const string& pipe, const string& cmdline) {
		using Cli = symbol::core::Hmi;
		ostringstream out;
		ostringstream err;
		if (pipe.empty()) {
			Cli::Section::init(out, err, cin, "0.0.0");
		}
		else {
			istringstream in(pipe);
			Cli::Section::init(out, err, in, "0.0.0");
		}
		Cli cli;
		cli.init("symbol-offline", "Symbol wallet - offline.");
		int r = cli.exec(cmdline)?0:1;
		return make_tuple(r, out.str(), err.str());
	}
}

tuple<int, string, string> test_keys_sk(const string& sk, bool pipe) {
	string cmd;
	if (pipe) {
		cmd="keys -s -";
	}
	else {
		ostringstream os;
		os << "keys -s " << sk;
		cmd=os.str();
	}
	return exec_offline(pipe?sk:"", cmd);
}


TEST(HMI_Keys, keys_sk_text) {
	/// Arrange
	auto sk="D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD";
	auto expected_cout=
		"networkId 152 (0x98)\n" \
		"network public-test\n" \
		"privateKey D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD\n" \
		"publicKey F6B15DE52C19649A397A3A8D38F8E7E10A7FA806FFD52D001319699C3509AB4B\n" \
		"address 984AFFFB78C25C0E1280DFA51A7CECDBBFA0C5D99CD932C9\n" \
		"account TBFP763YYJOA4EUA36SRU7HM3O72BROZTTMTFSI\n";
	auto expected_cerr="";

	/// Act
	auto r1=test_keys_sk(sk, false);
	auto r2=test_keys_sk(sk, true);

	/// Assert
	//cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r1), 0);
	EXPECT_EQ(get<1>(r1), expected_cout);
	EXPECT_EQ(get<2>(r1), expected_cerr);
	
	EXPECT_EQ(get<0>(r2), 0);
	EXPECT_EQ(get<1>(r2), expected_cout);
	EXPECT_EQ(get<2>(r2), expected_cerr);
}

TEST(HMI_Keys, keys_sk_etext) {
	/// Arrange
	auto sk="D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD";
	ostringstream cmd;
	cmd << "-o etext keys -s " << sk;

	/// Act
	auto r=exec_offline("", cmd.str());

	/// Assert
	auto expected_cout="152 public-test D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD F6B15DE52C19649A397A3A8D38F8E7E10A7FA806FFD52D001319699C3509AB4B 984AFFFB78C25C0E1280DFA51A7CECDBBFA0C5D99CD932C9 TBFP763YYJOA4EUA36SRU7HM3O72BROZTTMTFSI ";
	auto expected_cerr="";
	//cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r), 0);
	EXPECT_EQ(get<1>(r), expected_cout);
	EXPECT_EQ(get<2>(r), expected_cerr);
}

TEST(HMI_Keys, keys_sk_ejson) {
	/// Arrange
	auto sk="D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD";
	ostringstream cmd;
	cmd << "-o ejson keys -s " << sk;

	/// Act
	auto r=exec_offline("", cmd.str());

	/// Assert
	auto expected_cout="152 public-test D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD F6B15DE52C19649A397A3A8D38F8E7E10A7FA806FFD52D001319699C3509AB4B 984AFFFB78C25C0E1280DFA51A7CECDBBFA0C5D99CD932C9 TBFP763YYJOA4EUA36SRU7HM3O72BROZTTMTFSI ";
	auto expected_cerr="";
	cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r), 0);
	EXPECT_EQ(get<1>(r), expected_cout);
	EXPECT_EQ(get<2>(r), expected_cerr);
}


