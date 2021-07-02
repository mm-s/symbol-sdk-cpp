#!/bin/bash
prog=$0
jobs=4
if [[ -f /proc/cpuinfo ]]; then
	jobs=`cat /proc/cpuinfo | grep "^processor" | wc -l`
fi
#depsdir=""
hostaddr=`hostname -I | awk '{print $1'}`
#warn_env=0
#depsdir=$DEPS_DIR
#if [ "_$DEPS_DIR" == "_" ]; then
#	DEPS_DIR="$HOME/symbol_deps"
#	depsdir=$DEPS_DIR
#	echo "DEPS_DIR not found in env. Using default: $DEPS_DIR."
#	warn_env=1
#fi
#boost_output_dir=$depsdir/boost
#LD_LIBRARY_PATH_def="${depsdir}/boost/lib:${depsdir}/google/lib:./"
#LD_LIBRARY_PATH_def=""
debs="git gcc g++ cmake curl libssl-dev ninja-build rapidjson-dev libgtest-dev libboost-all-dev"

catapult_rep="https://github.com/nemtech/catapult-server"
catbuffer_rep="https://github.com/nemtech/catbuffer-generators"
catbuffer_parser_rep="https://github.com/nemtech/catbuffer-parser"
catbuffer_schemas_rep="https://github.com/nemtech/catbuffer-schemas.git"


function help {
	cat << EOF
$prog provides high level control.
Syntax: $prog [options] [command]

Options:
    -j <number>          Parallel compilation jobs. Default is $jobs

Available commands:
    make [release|debug]    Create and configure a new _build dir. Default RelWithDebInfo
    importcat               Import catapult sources.
    install system_reqs     Installs apt dependencies. Requires sudo.
                              debian packages: $debs
EOF
#    install deps            Compile & install 3rd party libs.
#    download deps           Obtain 3rd party libs.
#    [dev|test|main]net
#        create              Create new private-test network with 1 node at _build.
#        node <cmd>          Node management. cmd one of:
#            list            Current list of configured nodes, ordered by TCP port.
#            new <port>
#                            Add new [mainnet|testnet|devnet] node listening
#                              on the specified TCP port.
#            info <port>     Displays information about the node at specified TCP port.
#            rm <port>       Delete the node identified by its listening TCP port.
#            start <port>    Start node by listening TCP port.
#            stop <port>     Stop node by listening TCP port.
#        clean               Deletes private-test network at _build
#    cert generate <dir> [skM] [skT]   Tool for creating new certificates.
#                            If main or transport private keys are generated if not specified
#    conf                    generates $conf_file and exits.

#	if [ "_$DEPS_DIR" == "_" ]; then
#		echo "Environment variable DEPS_DIR not set. Required for storing source dependencies."
#		echo "Default value is: $HOME/symbol_deps"
#	else
#		echo "Dependencies env var DEPS_DIR is set to $DEPS_DIR"
#	fi
#	echo "Reminder: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_def"
}

function exitok {
	exit 0
#	if [ $warn_env -eq 1 ]; then
#cat << EOF
#Please export the environment variable DEPS_DIR
#
#  export DEPS_DIR=$depsdir
#
#Note: If you want the DEPS_DIR environment variable to persist across sessions make sure to include it in the ~/.profile or ~/.bashrc files.
#EOF
#	fi
#	exit 0
}

#function download_git_dependency {
#	git clone "git://github.com/${1}/${2}.git"
#	cd "${2}"
#	git checkout "${3}"
#	cd ..
#}

#function download_boost {
#	local boost_ver=1_${1}_0
#	local boost_ver_dotted=1.${1}.0
#	curl -o "boost_${boost_ver}.tar.gz" -SL "https://boostorg.jfrog.io/artifactory/main/release/${boost_ver_dotted}/source/boost_${boost_ver}.tar.gz"
#	tar -xzf "boost_${boost_ver}.tar.gz"
#	mv "boost_${boost_ver}" boost
#}

#function download_jsoncpp {
#	wget https://github.com/open-source-parsers/jsoncpp/archive/1.9.4.tar.gz
#	tar -xzf 1.9.4.tar.gz
#	rm 1.9.4.tar.gz -r
#}


#function download_all {
#	download_boost 76
#	download_git_dependency google googletest release-1.10.0
#	download_git_dependency google benchmark v1.5.3
#	download_jsoncpp
#}

#function install_git_dependency {
#	cd "${2}"
#	mkdir -p _build
#	cd _build
##	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX="$depsdir/${1}" "${cmake_options[@]}" ..
#	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$depsdir/${1}" "${cmake_options[@]}" ..
#	make -j $jobs && make install
#}

#function install_jsoncpp {
#	pushd jsoncpp-1.9.4 > /dev/null
#		mkdir -p _build
#		cd _build
#		cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$depsdir/${1}" "${cmake_options[@]}" ..
#		make -j $jobs && make install
#	popd > /dev/null
#}

#function install_boost {
#	pushd boost > /dev/null
#		./bootstrap.sh with-toolset=gcc --prefix="${boost_output_dir}"
#		b2_options=()
#		b2_options+=("--prefix=${boost_output_dir}")
#		./b2 "${b2_options[@]}" -j "$jobs" stage release
#		./b2 install "${b2_options[@]}"
#	popd > /dev/null
#}

#function install_google_test {
#	cmake_options=()
#	cmake_options+=("-DCMAKE_POSITION_INDEPENDENT_CODE=ON")
#	install_git_dependency google googletest
#}

#function install_google_benchmark {
#	cmake_options=()
#	cmake_options+=("-DBENCHMARK_ENABLE_GTEST_TESTS=OFF")
#	install_git_dependency google benchmark
#}

#function install_all {
#	declare -a installers=(
#		install_boost
#		install_google_test
#		install_google_benchmark
##		install_jsoncpp
#	)
#	for install in "${installers[@]}"
#	do
#		pushd source > /dev/null
#			${install}
#		popd > /dev/null
#	done
#}

function reqroot {
	if [ "_$(whoami)" != "_root" ]; then
		echo "Please run as root. (or use sudo)"
		exit 1
	fi
}

function install_system_reqs {
	reqroot
	set -e
	apt update
	apt -y install $debs
	set +e
}

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

#	find test/catapult \( -name "*.h" -o -name "*.cpp" \) -exec sed -i "s~^\(\t*\)#include \"catapult/.*/\(.*\)\"~\1#include <symbol/core/catapult/\2>~" {} \;
#	sed -i "s~^\(\t*\)#include \"catapult/.*/\(.*\)\"~\1#include <symbol/core/catapult/\2>~" test/catapult/TestHarness.h
	find test/catapult \( -name "*.h" -o -name "*.cpp" \) -exec sed -i "s~^\(\t*\)#include \"catapult.*/\(.*\)\"~\1#include <symbol/core/catapult/\2>~" {} \;
	sed -i "s~^\(\t*\)#include \"tests/test/nodeps/\(.*\)\"~\1#include \"\2\"~" test/catapult/TestHarness.h
#include "tests/test/nodeps/

	sed -i "s~^\(\t*\)#include \"tests/TestHarness.h\"~\1#include \"TestHarness.h\"~" test/catapult/Waits.h

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

	grab_classes catapult-server/src/catapult/utils core/catapult  ExceptionLogging MacroBasedEnum MacroBasedEnumIncludes ArraySet Hashers MemoryUtils ImmutableValue BaseValue ClampedBaseValue NonCopyable StreamFormatGuard HexFormatter HexParser FileSize IntegerMath TimeSpan Casting BlockSpan BitwiseEnum RawBuffer PathUtils Logging ConfigurationValueParsers Base32 ByteArray 
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


	mkdir -p test/catapult
	grab_classes catapult-server/tests/test/nodeps test/catapult Conversions Random Stress Waits
	grab_classes catapult-server/tests test/catapult  TestHarness

}

#function _load_write_defs {
#	file=$1
#	if [[ ! -f $file ]]; then
#		cat << EOF > $file
##uncomment to override
##depsdir=""
##hostaddr=$hostaddr
##depsdir=$depsdir
##LD_LIBRARY_PATH_def=$LD_LIBRARY_PATH_def
##debs=$debs
#EOF
#		echo -n "First time run: I have created defaults at $file. continue? (or CTRL-Z to edit the file. (type fg to return back)): "
#		read x
#	fi
#	. $file
#}

function prepare_sources2 {
	#produce_catbuffer_sources
	rm -rf catapult-server
	git clone --depth 1 $catapult_rep
	rm -rf core/catapult/*

	catapult1 #Network, Transactions, Crypto
	#catapult2 #Server Process

	replace_includes
}

function prepare_sources {
	if [[ -f core/catapult/NamespaceIdGenerator.cpp ]]; then
		echo "catapult sources are ready."
		return
	fi
	prepare_sources2
}

function make_builddir {
	echo "building using ${jobs} jobs"
#	echo "dependencies dir: ${depsdir}"
#	if [ ! -d "${boost_output_dir}" ]; then
#		install_deps
#	fi
#	echo "dependencies OK at: ${depsdir}"

	prepare_sources

	sep=";"
	if [[ "$OSTYPE" == "darwin"* ]]; then
		sep=":"
	fi

	bdir="_buildd"
	if [[ "_$1" == "_release" ]]; then
		bdir="_build"
	fi

	set -e
	mkdir -p $bdir
	pushd $bdir > /dev/null
		cmakeflags=""
		if [[ "_$1" == "_release" ]]; then
			cxxflags="-O3 -rdynamic"
			#cmakeflags="-DCMAKE_BUILD_TYPE=RelWithDebInfo"
			cmakeflags="-DCMAKE_BUILD_TYPE=Release"
		else
			if [[ "_$1" == "_debug" ]]; then
				cxxflags="-g -O0 -rdynamic"
				cmakeflags="-DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"
			else
				cxxflags="-O3 -rdynamic"
				cmakeflags="-DCMAKE_BUILD_TYPE=RelWithDebInfo"
			fi
		fi
		echo "Executing cmake with flags $cmakeflags $cxxflags"
		#BOOST_ROOT="${depsdir}/boost"
		#-DCMAKE_PREFIX_PATH="${depsdir}/google" \
		#-DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_PREFIX" \
		cmake .. \
		$cmakeflags \
		-DCMAKE_CXX_FLAGS="$cxxflags" \
		\
		-GNinja
		#ninja publish

		#Tweak options:
		#-DCMAKE_BUILD_TYPE=Debug \
		#-DCMAKE_CXX_FLAGS="-g -O0 -rdynamic" \
		#-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
	popd
	set +e
	echo "Sources are ready in directory $bdir"
	echo "Next: Compile:"
	echo "  cd $bdir"
	echo "  ninja -v -j${jobs}"
	echo "Success."
	exitok
}

force_download=0


#function download_deps {
#	if [ -d "$depsdir" ]; then
#		echo -n "Warning: ${depsdir} already exists. "
#		if [ ${force_download} -eq 0 ]; then
#			echo "Download skipped."
#			return
#		fi
#		echo ""
#	fi
#	mkdir -p "$depsdir"
#	set -e
#	pushd "$depsdir" > /dev/null
#		mkdir -p source
#		pushd source > /dev/null
#			download_all
#		popd
#	popd
#	set +e
#}

#function download {
#	cmd=$1
#	shift
#	if [ "_$cmd" == "_deps" ]; then
#		force_download=1
#		download_deps "$@"
#		exitok
#	fi
#}

#function install_deps {
#	if [ ! -d "${boost_output_dir}" ]; then
#		download_deps
#	fi
#	pushd "$depsdir" > /dev/null
#		install_all
#	popd
#}

function install_main {
	cmd=$1
	shift
	if [ "_$cmd" == "_system_reqs" ]; then
		install_system_reqs "$@"
		exitok
#	elif [ "_$cmd" == "_deps" ]; then
#		install_deps "$@"
#		exitok
	fi
}

cmd=""
while [ true ]; do
	opt=$1
	shift
	if [ "_$opt" == "_-j" ]; then
		jobs=$1
		shift
		echo "jobs $jobs"
		continue
	else
		cmd=$opt
		break
	fi
done

if [ "_$cmd" == "_install" ]; then
	install_main "$@"
#elif [ "_$cmd" == "_download" ]; then
#	download $@
elif [ "_$cmd" == "_importcat" ]; then
	prepare_sources2 $@
	exitok
elif [ "_$cmd" == "_make" ]; then
	make_builddir $@
elif [ "_$cmd" == "_" ]; then
	help
	exit 0
fi

echo "Error at $cmd"
#error flow
help
exit 1




