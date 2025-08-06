#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <openssl/sha.h>
#include <json/json.h>

using namespace std;

// Blockchain data structure
struct Block {
    string prevHash;
    string data;
    string hash;
    int index;
};

// dApp parser class
class dAppParser {
private:
    unordered_map<string, Block> blockchain;
    string currentHash;

public:
    dAppParser() {
        // Genesis block
        Block genesis;
        genesis.prevHash = "0";
        genesis.data = "Genesis block";
        genesis.index = 0;
        string genesisHash = calculateHash(genesis);
        genesis.hash = genesisHash;
        blockchain[genesisHash] = genesis;
        currentHash = genesisHash;
    }

    // Calculate SHA-256 hash
    string calculateHash(const Block& block) {
        string data = to_string(block.index) + block.prevHash + block.data;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*) data.c_str(), data.size(), hash);
        string hashStr = "";
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            char buf[3];
            sprintf(buf, "%02x", hash[i]);
            hashStr += buf;
        }
        return hashStr;
    }

    // Add new block to the blockchain
    void addBlock(const string& data) {
        Block newBlock;
        newBlock.prevHash = currentHash;
        newBlock.data = data;
        newBlock.index = blockchain.size();
        string newBlockHash = calculateHash(newBlock);
        newBlock.hash = newBlockHash;
        blockchain[newBlockHash] = newBlock;
        currentHash = newBlockHash;
    }

    // Get blockchain data
    Json::Value getBlockchainData() {
        Json::Value blockchainData;
        for (auto& block : blockchain) {
            Json::Value blockData;
            blockData["prevHash"] = block.second.prevHash;
            blockData["data"] = block.second.data;
            blockData["hash"] = block.second.hash;
            blockData["index"] = block.second.index;
            blockchainData.append(blockData);
        }
        return blockchainData;
    }
};

int main() {
    dAppParser parser;

    // Add blocks to the blockchain
    parser.addBlock("Transaction 1");
    parser.addBlock("Transaction 2");
    parser.addBlock("Transaction 3");

    // Get and display blockchain data
    Json::Value blockchainData = parser.getBlockchainData();
    cout << blockchainData << endl;

    return 0;
}