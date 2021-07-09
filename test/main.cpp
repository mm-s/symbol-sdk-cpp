#include <symbol/symbol.h>
#include <symbol/core.h>
#include <iostream>
#include <cassert>
#include <gtest/gtest.h>
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

