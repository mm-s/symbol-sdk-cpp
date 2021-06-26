#!/bin/bash

catapult_rep="https://github.com/nemtech/catapult-server"
catbuffer_rep="https://github.com/nemtech/catbuffer-generators"
catbuffer_parser_rep="https://github.com/nemtech/catbuffer-parser"
catbuffer_schemas_rep="https://github.com/nemtech/catbuffer-schemas.git"

function produce_catbuffer_sources {
	rm -rf tmp
	mkdir tmp
	pushd tmp > /dev/null
		git clone --depth 1 $catbuffer_rep
		pushd catbuffer-generators > /dev/null
			git clone --depth 1 $catbuffer_schemas_rep
			#sed -i "s/all_transactions.cats/all.cats/" scripts/generate_cpp.sh
			cat scripts/generate_cpp.sh
			scripts/generate_cpp.sh
			#exit 1
		popd > /dev/null
	popd > /dev/null
	mv tmp/catbuffer-generators/build/cpp/cpp-transaction-builders/src/* core/catapult/
	rm -rf tmp
}

function grab_classes {
	src=$1
	shift
	dst=$1
	shift
	for n in $@; do
		echo "class: $n"
		[[ ! -f $src/$n.h ]] && echo "$n.h not found in $src" && exit 1
		#[[ -f $dst/$n.h ]] && echo "$n.h already exists in $dst" && exit 1
		mv $src/$n.h $dst/
		[[ ! -f $src/$n.cpp ]] && continue
		#[[ -f $dst/$n.cpp ]] && echo "$n.cpp already exists in $dst" && exit 1
		mv $src/$n.cpp $dst/
	done
}

function replace_includes {
	find core/catapult \( -name "*.h" -o -name "*.cpp" \) -exec sed -i "s~^\(\t*\)#include \".*/\(.*\)\"~\1#include \"\2\"~" {} \;

	sed -i "s~^#include \"catapult.h\"~#include \"../donna/catapult.h\"~" core/catapult/CryptoUtils.cpp
	sed -i "s~^#include \"catapult.h\"~#include \"../donna/catapult.h\"~" core/catapult/KeyPair.cpp
	sed -i "s~^#include \"catapult.h\"~#include \"../donna/catapult.h\"~" core/catapult/Signer.cpp
	sed -i "s~^#include \"catapult.h\"~#include \"../donna/catapult.h\"~" core/catapult/Vrf.cpp
}

function append_h {
	src=$1
	dst=$2
	name=$(basename $dst)
	touch $dst
	let n=`cat $dst | wc -l`
	echo "#include \"${name}.${n}.h\"" >> $dst
	cat $src >> "$dst.${n}.h"
	rm $src
}

#function catapultnew {
#	grab_classes catapult-server/src/catapult/subscribers core/catapult  
#	grab_classes catapult-server/src/catapult/consumers core/catapult  
#	grab_classes catapult-server/src/catapult/io core/catapult  #
#	grab_classes catapult-server/src/catapult/cache core/catapult  
#	grab_classes catapult-server/src/catapult/model core/catapult  
#	grab_classes catapult-server/src/catapult/local/server core/catapult  
#	grab_classes catapult-server/src/catapult/cache_tx core/catapult  
#	grab_classes catapult-server/src/catapult/model core/catapult  
#	grab_classes catapult-server/src/catapult/utils core/catapult  
#	grab_classes catapult-server/src/catapult/chain core/catapult  
#	grab_classes catapult-server/src/catapult/ionet core/catapult  
#	grab_classes catapult-server/src/catapult/thread core/catapult  
#	grab_classes catapult-server/src/catapult/observers core/catapult  
#	grab_classes catapult-server/src/catapult/validators core/catapult  
#	grab_classes catapult-server/src/catapult/disruptor core/catapult  
#}

function catapult2 {
	grab_classes catapult-server/src/catapult/extensions core/catapult  TransactionEvent BasicServerHooks ServerHooks ProcessBootstrapper
	grab_classes catapult-server/src/catapult/local/server core/catapult  NodeContainerSubscriberAdapter NemesisBlockNotifier MemoryCounters LocalNode FileStateChangeStorage
	grab_classes catapult-server/src/catapult/local core/catapult  ProcessHost
	grab_classes catapult-server/src/catapult/process core/catapult  ProcessMain
	grab_classes catapult-server/src/catapult/utils core/catapult  SpinLock SpinReaderWriterLock ShortHash DiagnosticCounter DiagnosticCounterId

	grab_classes catapult-server/src/catapult/io core/catapult  PodIoUtils Stream
	grab_classes catapult-server/src/catapult/subscribers core/catapult  SubscriberOperationTypes StateChangeInfo StateChangeSubscriber

	grab_classes catapult-server/src/catapult/cache core/catapult  ReadOnlyCatapultCache SubCachePlugin StateHashInfo CacheChangesStorage CacheChanges CatapultCacheDelta 
	grab_classes catapult-server/src/catapult/model core/catapult  HeightHashPair NetworkInfo HeightGrouping NotificationContext BlockStatementBuilder NotificationPublisher AnnotatedEntityRange EntityRange RangeTypes ChainScore
	grab_classes catapult-server/src/catapult/cache_tx core/catapult  BasicTransactionsCache UtCache MemoryCacheProxy MemoryCacheOptions MemoryUtCache
	grab_classes catapult-server/src/catapult/chain core/catapult  ExecutionConfiguration BatchEntityProcessor RemoteNodeSynchronizer ChainSynchronizer ChainFunctions
	grab_classes catapult-server/src/catapult/ionet core/catapult  NodeInteractionResultCode
	grab_classes catapult-server/src/catapult/thread core/catapult  Future FutureUtils
	grab_classes catapult-server/src/catapult/thread/detail core/catapult  FutureSharedState
	grab_classes catapult-server/src/catapult/disruptor core/catapult  DisruptorConsumer InputSource ConsumerInput DisruptorElement DisruptorTypes
	grab_classes catapult-server/src/catapult/consumers core/catapult  InputUtils HashCheckOptions BlockChainSyncHandlers BlockChainProcessor BlockConsumers
	grab_classes catapult-server/src/catapult/observers core/catapult  FunctionalNotificationObserver EntityObserver ObserverStatementBuilder ObserverContext NotificationObserver AggregateNotificationObserver ObserverTypes
	grab_classes catapult-server/src/catapult/state core/catapult  CatapultState
	grab_classes catapult-server/src/catapult/validators core/catapult FunctionalNotificationValidator EntityValidator ValidatorContext ValidationResult NotificationValidator AggregateNotificationValidator ValidatorTypes

}

function catapult1 {
	mv catapult-server/sdk/src/builders/* core/catapult/

	grab_classes catapult-server/src/catapult core/catapult  preprocessor functions plugins exceptions types
	grab_classes catapult-server/src/catapult/crypto core/catapult  AesDecrypt SecureRandomGenerator SharedKey CryptoUtils Vrf Signer SecureZero SecureByteArray BasicKeyPair KeyPair OpensslContexts Hashes MerkleHashBuilder

	grab_classes catapult-server/src/catapult/utils core/catapult  MacroBasedEnum MacroBasedEnumIncludes ArraySet Hashers MemoryUtils ImmutableValue BaseValue ClampedBaseValue NonCopyable StreamFormatGuard HexFormatter HexParser FileSize IntegerMath TimeSpan Casting BlockSpan BitwiseEnum RawBuffer PathUtils Logging ConfigurationValueParsers Base32 ByteArray 
	grab_classes catapult-server/src/catapult/utils/traits core/catapult  Traits

	grab_classes catapult-server/src/catapult/model core/catapult  Cosignature KeyLinkSharedTransaction EntityInfo Mosaic Receipt TransactionStatement ReceiptType ReceiptSource ResolutionStatement BlockStatement Elements EntityHasher WeakEntityInfo PluginRegistry TransactionRegistry ResolverContext TransactionPlugin Resolvable NotificationType LinkAction ContainerTypes Notifications NotificationSubscriber Address VerifiableEntity NetworkIdentifier FacilityCode EntityType EntityBody EmbeddedTransaction Transaction SizeChecker SizePrefixedEntity ContiguousEntityContainer SizePrefixedEntityContainer Block NodeIdentity 
	grab_classes catapult-server/plugins/txes/aggregate/src/model core/catapult  AggregateEntityType AggregateTransaction

	grab_classes catapult-server/plugins/txes/mosaic/src/model core/catapult  MosaicReceiptType MosaicTypes MosaicSupplyChangeTransaction MosaicFlags MosaicProperties MosaicEntityType MosaicDefinitionTransaction MosaicConstants MosaicIdGenerator 
	append_h catapult-server/plugins/txes/mosaic/src/types.h core/catapult/types.h

	grab_classes catapult-server/plugins/txes/restriction_account/src/model/ core/catapult AccountOperationRestrictionTransaction AccountMosaicRestrictionTransaction AccountRestrictionFlags AccountRestrictionEntityType AccountRestrictionSharedTransaction AccountAddressRestrictionTransaction

	grab_classes catapult-server/plugins/txes/account_link/src/model/ core/catapult  NodeKeyLinkTransaction AccountLinkEntityType AccountKeyLinkTransaction

	grab_classes catapult-server/plugins/txes/metadata/src/model/ core/catapult NamespaceMetadataTransaction MosaicMetadataTransaction MetadataSharedTransaction MetadataEntityType AccountMetadataTransaction

	grab_classes catapult-server/plugins/txes/namespace/src/model/ core/catapult  NamespaceReceiptType NamespaceIdGenerator NamespaceConstants NamespaceRegistrationTransaction MosaicAliasTransaction NamespaceTypes NamespaceEntityType AddressAliasTransaction
	append_h catapult-server/plugins/txes/namespace/src/types.h core/catapult/types.h
	append_h catapult-server/plugins/txes/namespace/src/constants.h core/catapult/constants.h

	grab_classes catapult-server/plugins/txes/lock_hash/src/model/ core/catapult  HashLockReceiptType HashLockEntityType HashLockTransaction

	grab_classes catapult-server/plugins/txes/restriction_mosaic/src/model/ core/catapult MosaicRestrictionTypes MosaicGlobalRestrictionTransaction MosaicRestrictionEntityType MosaicAddressRestrictionTransaction

	grab_classes catapult-server/plugins/txes/lock_secret/src/model/ core/catapult  SecretLockReceiptType SecretProofTransaction LockHashAlgorithm SecretLockTransaction SecretLockEntityType

	grab_classes catapult-server/plugins/txes/multisig/src/model/ core/catapult MultisigEntityType MultisigAccountModificationTransaction

	grab_classes catapult-server/plugins/txes/transfer/src/model/ core/catapult TransferTransaction TransferEntityType

	grab_classes catapult-server/plugins/coresystem/src/model/ core/catapult VrfKeyLinkTransaction VotingKeyLinkTransaction

	grab_classes catapult-server/sdk/src/extensions/ core/catapult TransactionExtensions

	grab_classes catapult-server/src/catapult/utils/ core/catapult ConfigurationBag ConfigurationUtils
	grab_classes catapult-server/src/catapult/config/ core/catapult LoggingConfiguration ConfigurationFileLoader PeersConfiguration
	grab_classes catapult-server/src/catapult/ionet/ core/catapult Node NodeRoles NodeVersion
	grab_classes catapult-server/src/catapult/version/ core/catapult version version_inc

	grab_classes catapult-server/src/catapult/io core/catapult  Stream BufferInputStreamAdapter EntityIoUtils PodIoUtils


	mkdir -p core/donna
	mv catapult-server/external/donna/*.c core/donna/
	mv catapult-server/external/donna/*.h core/donna/

	sed -i "s~#include <donna/catapult.h>~#include \"../donna/catapult.h\"~" core/catapult/CryptoUtils.cpp
	sed -i "s~#include <donna/catapult.h>~#include \"../donna/catapult.h\"~" core/catapult/KeyPair.cpp
	sed -i "s~#include <donna/catapult.h>~#include \"../donna/catapult.h\"~" core/catapult/Signer.cpp
	sed -i "s~#include <donna/catapult.h>~#include \"../donna/catapult.h\"~" core/catapult/Vrf.cpp
	sed -i "s~#include <donna/\(.*\).h>~#include \"\1.h\"~" core/donna/catapult.h

	sed -i "s/class BasicKeyPair final/class BasicKeyPair/" core/catapult/BasicKeyPair.h
	sed -i "s/private:/protected:/" core/catapult/BasicKeyPair.h

}

#catapultnew
#replace_includes
#exit 1

#produce_catbuffer_sources
rm -rf catapult-server
git clone --depth 1 $catapult_rep
rm -rf core/catapult/*

catapult1 #Network, Transactions, Crypto
#catapult2 #Server Process

replace_includes

echo "Success: type make to create _buildd dir"

exit 0

