#include <symbol/symbol.h>
#include <symbol/core.h>
#include <iostream>
#include <cassert>
#include <gtest/gtest.h>
#include <symbol/core.h>
#include "hmi.h"
/*
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
*/
using std::tuple;
using namespace symbol::test::hmi;


TEST(HMI, empty_args) {
	/// Arrange
	auto expected_cout="";
	auto expected_cerr="ERR Missing command.\n\nsymbol-offline  r0.0.0\n\nSymbol wallet - offline.\n\nusage:\n  symbol-offline [flags] <command> [-h]\n\nflags:\n  -H, --home (value='~/.symbol')     Home directory.\n  -v, --verbose                      Print extra information about what the program is doing.\n  -o, --output (value='text')        Output format. -h for list of valid options.\n  -n, --network (value='public-test')  Network type.\n  -s, --seed <value>                 Network generation hash seed.\n  -B, --blob <value>                 Memory representation. Hex string.\n  -h, --help                         Prints this help.\n\ncommands:\n  keys                               Keys/Address/Account generation/info.\n  tx                                 Operations with transactions.\n  wallet                             Private.\n";
	auto expected_exit_code=1;

	/// Act
	auto r = exec_offline("", "");

	/// Assert
	//cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r), expected_exit_code);
	EXPECT_EQ(get<1>(r), expected_cout);
	EXPECT_EQ(get<2>(r), expected_cerr);
}

TEST(HMI, invoke_help) {
	/// Arrange
	auto expected_cout="symbol-offline  r0.0.0\n\nSymbol wallet - offline.\n\nusage:\n  symbol-offline [flags] <command> [-h]\n\nflags:\n  -H, --home (value='~/.symbol')     Home directory.\n  -v, --verbose                      Print extra information about what the program is doing.\n  -o, --output (value='text')        Output format. -h for list of valid options.\n  -n, --network (value='public-test')  Network type.\n  -s, --seed <value>                 Network generation hash seed.\n  -B, --blob <value>                 Memory representation. Hex string.\n  -h, --help (flag is set)           Prints this help.\n\ncommands:\n  keys                               Keys/Address/Account generation/info.\n  tx                                 Operations with transactions.\n  wallet                             Private.\n";
	auto expected_cerr="";
	auto expected_exit_code=0;

	/// Act
	auto r = exec_offline("", "-h");

	/// Assert
	//cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r), expected_exit_code);
	EXPECT_EQ(get<1>(r), expected_cout);
	EXPECT_EQ(get<2>(r), expected_cerr);
}


