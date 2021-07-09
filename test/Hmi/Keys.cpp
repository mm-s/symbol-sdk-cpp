#include <symbol/symbol.h>
#include <symbol/core.h>
#include <iostream>
#include <cassert>
#include <gtest/gtest.h>
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
using std::string;
using std::ostringstream;

tuple<int, string, string> test_keys_sk(const string& sk, const string& output, bool pipe) {
	ostringstream cmd;
	if (!output.empty()) {
		cmd << "-o " << output << ' ';
	}
	if (pipe) {
		cmd << "keys -s -";
	}
	else {
		cmd << "keys -s " << sk;
	}
	return symbol::test::hmi::exec_offline(pipe?sk:"", cmd.str());
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
	auto r1=test_keys_sk(sk, "", false);
	auto r2=test_keys_sk(sk, "", true); //pipe sk

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
	auto expected_cout="152 public-test D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD F6B15DE52C19649A397A3A8D38F8E7E10A7FA806FFD52D001319699C3509AB4B 984AFFFB78C25C0E1280DFA51A7CECDBBFA0C5D99CD932C9 TBFP763YYJOA4EUA36SRU7HM3O72BROZTTMTFSI ";
	auto expected_cerr="";

	/// Act
	auto r1=test_keys_sk(sk, "etext", false);
	auto r2=test_keys_sk(sk, "etext", true); //pipe sk

	/// Assert
	//cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r1), 0);
	EXPECT_EQ(get<1>(r1), expected_cout);
	EXPECT_EQ(get<2>(r1), expected_cerr);
	
	EXPECT_EQ(get<0>(r2), 0);
	EXPECT_EQ(get<1>(r2), expected_cout);
	EXPECT_EQ(get<2>(r2), expected_cerr);
}

TEST(HMI_Keys, keys_sk_ejson) {
	/// Arrange
	auto sk="D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD";
	ostringstream cmd;
	cmd << "-o ejson keys -s " << sk;
	auto expected_cout="{\"networkId\":152,\"network\":\"public-test\",\"privateKey\":\"D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD\",\"publicKey\":\"F6B15DE52C19649A397A3A8D38F8E7E10A7FA806FFD52D001319699C3509AB4B\",\"address\":\"984AFFFB78C25C0E1280DFA51A7CECDBBFA0C5D99CD932C9\",\"account\":\"TBFP763YYJOA4EUA36SRU7HM3O72BROZTTMTFSI\"}";
	auto expected_cerr="";

	/// Act
	auto r1=test_keys_sk(sk, "ejson", false);
	auto r2=test_keys_sk(sk, "ejson", true); //pipe sk

	/// Assert
	//cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r1), 0);
	EXPECT_EQ(get<1>(r1), expected_cout);
	EXPECT_EQ(get<2>(r1), expected_cerr);
	
	EXPECT_EQ(get<0>(r2), 0);
	EXPECT_EQ(get<1>(r2), expected_cout);
	EXPECT_EQ(get<2>(r2), expected_cerr);
}

TEST(HMI_Keys, keys_sk_json) {
	/// Arrange
	auto sk="D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD";
	ostringstream cmd;
	cmd << "-o json keys -s " << sk;
	auto expected_cout="{\n    \"networkId\": 152,\n    \"network\": \"public-test\",\n    \"privateKey\": \"D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9BD\",\n    \"publicKey\": \"F6B15DE52C19649A397A3A8D38F8E7E10A7FA806FFD52D001319699C3509AB4B\",\n    \"address\": \"984AFFFB78C25C0E1280DFA51A7CECDBBFA0C5D99CD932C9\",\n    \"account\": \"TBFP763YYJOA4EUA36SRU7HM3O72BROZTTMTFSI\"\n}\n";
	auto expected_cerr="";

	/// Act
	auto r1=test_keys_sk(sk, "json", false);
	auto r2=test_keys_sk(sk, "json", true); //pipe sk

	/// Assert
	//cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r1), 0);
	EXPECT_EQ(get<1>(r1), expected_cout);
	EXPECT_EQ(get<2>(r1), expected_cerr);
	
	EXPECT_EQ(get<0>(r2), 0);
	EXPECT_EQ(get<1>(r2), expected_cout);
	EXPECT_EQ(get<2>(r2), expected_cerr);
}

TEST(HMI_Keys, keys_malformed_sk) {
	/// Arrange
	auto sk="D09068D3706B1C94E29D23A012D93331440D208015C9F6F823DFEA9B77E1D9B";
	ostringstream cmd;
	cmd << "keys -s " << sk;
	auto expected_cout="";
	auto expected_cerr="ERR Input is not a private key.\n";
	auto expected_exit_code=1;

	/// Act
	auto r1=test_keys_sk(sk, "", false);
	auto r2=test_keys_sk(sk, "", true); //pipe sk

	/// Assert
	//cout << get<1>(r) << endl;
	EXPECT_EQ(get<0>(r1), expected_exit_code);
	EXPECT_EQ(get<1>(r1), expected_cout);
	EXPECT_EQ(get<2>(r1), expected_cerr);

	EXPECT_EQ(get<0>(r2), expected_exit_code);
	EXPECT_EQ(get<1>(r2), expected_cout);
	EXPECT_EQ(get<2>(r2), expected_cerr);
}


