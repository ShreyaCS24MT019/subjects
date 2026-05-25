#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include <random>
#include <chrono>
#include <mutex>
#include <openssl/evp.h>

using namespace std;

class Block {
public:
    string parentHash;
    int nonce;
    string difficulty;
    long long timestamp;
    string merkleRoot;
    vector<string> transactions;
    string hash;

    Block(const string& prevHash = "", int nonceValue = 1000) {
        parentHash = prevHash;
        nonce = nonceValue;
        difficulty = string(40, '0');
        timestamp = time(nullptr);
        transactions = {"Dummy Transaction 1", "Dummy Transaction 2"};
        merkleRoot = calculateMerkleRoot();
        hash = calculateBlockHash();
    }

    string calculateMerkleRoot() {
        string concatenated;
        for (const auto& tx : transactions) {
            concatenated += tx;
        }
        return sha256(concatenated);
    }

    string calculateBlockHash() {
        string hashInput = parentHash +
                           to_string(nonce) +
                           difficulty +
                           to_string(timestamp) +
                           merkleRoot;
        return sha256(hashInput);
    }

private:
    string sha256(const string& input) {
        EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
        const EVP_MD* md = EVP_sha256();
        unsigned char md_value[EVP_MAX_MD_SIZE];
        unsigned int md_len;

        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, input.c_str(), input.length());
        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
        EVP_MD_CTX_free(mdctx);

        stringstream ss;
        for(unsigned int i = 0; i < md_len; i++) {
            ss << hex << setw(2) << setfill('0') << (int)md_value[i];
        }
        return ss.str();
    }
};

class Blockchain {
private:
    unordered_map<string, Block> blocks;
    string tip;
    mutex blockchainMutex;

public:
    Blockchain() {
        Block genesisBlock;
        cout << "Genesis block created with hash: " << genesisBlock.hash << endl;
        blocks[genesisBlock.hash] = genesisBlock;
        tip = genesisBlock.hash;
    }

    Block addBlock() {
        Block newBlock(tip);
        lock_guard<mutex> lock(blockchainMutex);
        cout << "------ New Block ------ " << endl;
        cout << "Parent Hash: " << newBlock.parentHash << endl;
        cout << "Nonce: " << newBlock.nonce << endl;
        cout << "Difficulty: " << newBlock.difficulty << endl;
        cout << "Timestamp: " << newBlock.timestamp << endl;
        cout << "Merkle Root: " << newBlock.merkleRoot << endl;
        cout << "Hash: " << newBlock.hash << endl;
        cout << "Current Blockchain Height: " << blocks.size() << endl;
        cout << "-----------------------------" << endl;
       
        blocks[newBlock.hash] = newBlock;
        tip = newBlock.hash;
       
        return newBlock;
    }

    string getTip() const {
        return tip;
    }

    void displayBlockchainHashes() {
        cout << "Blockchain Hashes:" << endl;
        for (const auto& pair : blocks) {
            cout << pair.first << endl;
        }
    }

    size_t getBlockchainSize() const {
        return blocks.size();
    }
};

void minerThread(Blockchain& blockchain, int minerId) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> sleepDist(100, 500);

    for (int i = 0; i < 10; ++i) {
        this_thread::sleep_for(chrono::milliseconds(sleepDist(gen)));
       
        blockchain.addBlock();
       
        cout << "Miner " << minerId << " added block " << i+1 << endl;
    }
}

int main() {
    Blockchain blockchain;
    vector<thread> miners;
    for (int i = 0; i < 5; ++i) {
        miners.emplace_back(minerThread, ref(blockchain), i);
    }
    for (auto& miner : miners) {
        miner.join();
    }
    cout << "\nFinal Blockchain Details:" << endl;
    cout << "Current Blockchain Tip: " << blockchain.getTip() << endl;
    cout << "Blockchain Size: " << blockchain.getBlockchainSize() << endl;
   
    blockchain.displayBlockchainHashes();

    return 0;
}


/* output

./blockchain_task2
Genesis block created with hash: 717ba83145c4140b055cf4780eb47a95614e7f9ca9f04c198ae18033ffca6e88
------ New Block ------ 
Parent Hash: 717ba83145c4140b055cf4780eb47a95614e7f9ca9f04c198ae18033ffca6e88
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 3148926bf46b5410c9b51eb1faa481bbf930dc92b9542f779749dc67aae2ab0e
Current Blockchain Height: 1
-----------------------------
Miner 4 added block 1
------ New Block ------ 
Parent Hash: 3148926bf46b5410c9b51eb1faa481bbf930dc92b9542f779749dc67aae2ab0e
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 4ffbcc03cbecaa71cd565d46aa792ff4f0ca3a7e174e7175b84570c1b4c4198f
Current Blockchain Height: 2
-----------------------------
Miner 0 added block 1
------ New Block ------ 
Parent Hash: 4ffbcc03cbecaa71cd565d46aa792ff4f0ca3a7e174e7175b84570c1b4c4198f
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 50dccf0c204371ba8275b0c3433f5a4af2316ed742b915e85894bc6f48b74031
Current Blockchain Height: 3
-----------------------------
Miner 3 added block 1
------ New Block ------ 
Parent Hash: 50dccf0c204371ba8275b0c3433f5a4af2316ed742b915e85894bc6f48b74031
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 7ee95742c86c5725eec60206e2ffa8861ff70cedb4549dc1905367c2c31fc5ee
Current Blockchain Height: 4
-----------------------------
Miner 0 added block 2
------ New Block ------ 
Parent Hash: 7ee95742c86c5725eec60206e2ffa8861ff70cedb4549dc1905367c2c31fc5ee
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 640444babb57db29b765d16006309ac2ed0ea1be8286eafac44957a97db31ffb
Current Blockchain Height: 5
-----------------------------
Miner 3 added block 2
------ New Block ------ 
Parent Hash: 640444babb57db29b765d16006309ac2ed0ea1be8286eafac44957a97db31ffb
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 7ef1f9d661e3e7cdb30c9cd95d20faa926a136355f6e3e370da2711936416010
Current Blockchain Height: 6
-----------------------------
Miner 4 added block 2
------ New Block ------ 
Parent Hash: 7ef1f9d661e3e7cdb30c9cd95d20faa926a136355f6e3e370da2711936416010
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 96c72b37626a66c94a0a8120917189e9563f1d17ad56f23ad05693de0d0489dc
Current Blockchain Height: 7
-----------------------------
Miner 2 added block 1
------ New Block ------ 
Parent Hash: 96c72b37626a66c94a0a8120917189e9563f1d17ad56f23ad05693de0d0489dc
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 9e0f95a12b67c059c9f242a8ae4f9ff331cbd968b50990879fcdf1c17b3ac6cc
Current Blockchain Height: 8
-----------------------------
Miner 3 added block 3
------ New Block ------ 
Parent Hash: 9e0f95a12b67c059c9f242a8ae4f9ff331cbd968b50990879fcdf1c17b3ac6cc
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778754
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: cf6b92d56d2c8fa61e342979c57ee7d512744f5585da51438f82cee9a29d260a
Current Blockchain Height: 9
-----------------------------
Miner 1 added block 1
------ New Block ------ 
Parent Hash: cf6b92d56d2c8fa61e342979c57ee7d512744f5585da51438f82cee9a29d260a
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 0bb538f4d735bbc8bc7c2c0089577aec2ad4aa4c821913148df7f42929eafa37
Current Blockchain Height: 10
-----------------------------
Miner 2 added block 2
------ New Block ------ 
Parent Hash: 0bb538f4d735bbc8bc7c2c0089577aec2ad4aa4c821913148df7f42929eafa37
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: bee3d35ad917d46d246a65a735c4ef17693cce9a9cdc552e007a18f93e022228
Current Blockchain Height: 11
-----------------------------
Miner 1 added block 2
------ New Block ------ 
Parent Hash: bee3d35ad917d46d246a65a735c4ef17693cce9a9cdc552e007a18f93e022228
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 652856549f78ab159e33051b62ec710948ae5c794edc0fbe24aab315e7ff9122
Current Blockchain Height: 12
-----------------------------
Miner 0 added block 3
------ New Block ------ 
Parent Hash: 652856549f78ab159e33051b62ec710948ae5c794edc0fbe24aab315e7ff9122
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: b2d81b94c3f1dcdb23ee4fbcfdb43aa5b25f29effa7d8b0563fe8bdbff22e01b
Current Blockchain Height: 13
-----------------------------
Miner 3 added block 4
------ New Block ------ 
Parent Hash: b2d81b94c3f1dcdb23ee4fbcfdb43aa5b25f29effa7d8b0563fe8bdbff22e01b
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: b01fb95db56cd4f4380c2531a782773f731fd7481eee011a927babf523aab7ee
Current Blockchain Height: 14
-----------------------------
Miner 4 added block 3
------ New Block ------ 
Parent Hash: b01fb95db56cd4f4380c2531a782773f731fd7481eee011a927babf523aab7ee
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 8f8f70e4d418d40bb6a3b9463ca093bb0e1fb74b5077f0efe97d28358e26f348
Current Blockchain Height: 15
-----------------------------
Miner 1 added block 3
------ New Block ------ 
Parent Hash: 8f8f70e4d418d40bb6a3b9463ca093bb0e1fb74b5077f0efe97d28358e26f348
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 0ad75d7cabc7e93e929fa6dafd8f13019504e3c6c8d8c3042cc90a0a3f513b38
Current Blockchain Height: 16
-----------------------------
Miner 0 added block 4
------ New Block ------ 
Parent Hash: 0ad75d7cabc7e93e929fa6dafd8f13019504e3c6c8d8c3042cc90a0a3f513b38
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: a14d816b96e0d432e8d567bf37f0095ccc50a164e9a8f50a0c249603b3e1ff32
Current Blockchain Height: 17
-----------------------------
Miner 2 added block 3
------ New Block ------ 
Parent Hash: a14d816b96e0d432e8d567bf37f0095ccc50a164e9a8f50a0c249603b3e1ff32
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: a891392c95c8278629ccd58a719d6d33d2c97eaa1163260e32b7fb43f7a44e45
Current Blockchain Height: 18
-----------------------------
Miner 4 added block 4
------ New Block ------ 
Parent Hash: a891392c95c8278629ccd58a719d6d33d2c97eaa1163260e32b7fb43f7a44e45
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 888203f3475d49830605b34ecdaadcac5bda28effeb0d438035e32ae8748387b
Current Blockchain Height: 19
-----------------------------
Miner 2 added block 4
------ New Block ------ 
Parent Hash: 888203f3475d49830605b34ecdaadcac5bda28effeb0d438035e32ae8748387b
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: f02c8585c05b9be00ab2936c2f9de1e82f30af34f556ba9b8c25afee7a1bd678
Current Blockchain Height: 20
-----------------------------
Miner 3 added block 5
------ New Block ------ 
Parent Hash: f02c8585c05b9be00ab2936c2f9de1e82f30af34f556ba9b8c25afee7a1bd678
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 71f26ef996c2e37853c15e1b817b986cbc1c6b19954b33983086f4f519af014d
Current Blockchain Height: 21
-----------------------------
Miner 0 added block 5
------ New Block ------ 
Parent Hash: 71f26ef996c2e37853c15e1b817b986cbc1c6b19954b33983086f4f519af014d
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: c62974548342118891e714ffc1cca7f3a5674ad47b9f64e262a4092be35a6b55
Current Blockchain Height: 22
-----------------------------
Miner 1 added block 4
------ New Block ------ 
Parent Hash: c62974548342118891e714ffc1cca7f3a5674ad47b9f64e262a4092be35a6b55
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: eb4dbb7bf73274bbcccf980788956dcecee6018a5e10c757d33d139499ee9c50
Current Blockchain Height: 23
-----------------------------
Miner 4 added block 5
------ New Block ------ 
Parent Hash: eb4dbb7bf73274bbcccf980788956dcecee6018a5e10c757d33d139499ee9c50
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 635fb61b6e3bac2c072c5f726a9b37c378a5922f1cc0670ad07e8935528c6155
Current Blockchain Height: 24
-----------------------------
Miner 1 added block 5
------ New Block ------ 
Parent Hash: 635fb61b6e3bac2c072c5f726a9b37c378a5922f1cc0670ad07e8935528c6155
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778755
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 474a61ee0fb681b1436cd3fbd234c38694d729926ba1b415d27cee1ac04abde2
Current Blockchain Height: 25
-----------------------------
Miner 0 added block 6
------ New Block ------ 
Parent Hash: 474a61ee0fb681b1436cd3fbd234c38694d729926ba1b415d27cee1ac04abde2
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 5c59f7c1b362e57e21e752fe0ceb9a27673a8eb3aa2973b5e143752465716166
Current Blockchain Height: 26
-----------------------------
Miner 3 added block 6
------ New Block ------ 
Parent Hash: 5c59f7c1b362e57e21e752fe0ceb9a27673a8eb3aa2973b5e143752465716166
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 49d0db1daf4d8a5ddf63965c4ad4f98369f686ee80488b22723dd85e975631f2
Current Blockchain Height: 27
-----------------------------
Miner 2 added block 5
------ New Block ------ 
Parent Hash: 49d0db1daf4d8a5ddf63965c4ad4f98369f686ee80488b22723dd85e975631f2
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 19ca652cb5e12abd26c5e5000aa56541b3d6ffd41d7083d23472eb789135e42f
Current Blockchain Height: 28
-----------------------------
Miner 4 added block 6
------ New Block ------ 
Parent Hash: 19ca652cb5e12abd26c5e5000aa56541b3d6ffd41d7083d23472eb789135e42f
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 808b16ea7167518d6624229bb4235ca0e127b94320782baab3529d3bfb80fac6
Current Blockchain Height: 29
-----------------------------
Miner 1 added block 6
------ New Block ------ 
Parent Hash: 808b16ea7167518d6624229bb4235ca0e127b94320782baab3529d3bfb80fac6
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: f5317ffdfcabbb50e87932d425ed8d3345ad23ee8b2689dc2dfe480a071b59b7
Current Blockchain Height: 30
-----------------------------
Miner 0 added block 7
------ New Block ------ 
Parent Hash: f5317ffdfcabbb50e87932d425ed8d3345ad23ee8b2689dc2dfe480a071b59b7
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: cd49350b7ffd69a6cd43d5d5f2e12bcd6d0c8f57fed3f5140754de936f85ada2
Current Blockchain Height: 31
-----------------------------
Miner 2 added block 6
------ New Block ------ 
Parent Hash: cd49350b7ffd69a6cd43d5d5f2e12bcd6d0c8f57fed3f5140754de936f85ada2
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: e1a4d0c9d4abec081fc10222bf4ee5360ceba53dc596c95e223cab91072c7857
Current Blockchain Height: 32
-----------------------------
Miner 1 added block 7
------ New Block ------ 
Parent Hash: e1a4d0c9d4abec081fc10222bf4ee5360ceba53dc596c95e223cab91072c7857
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 315f5c2b7d434f6c713005a2baaae3e69b9c30761eded56768e1c991c172910f
Current Blockchain Height: 33
-----------------------------
Miner 0 added block 8
------ New Block ------ 
Parent Hash: 315f5c2b7d434f6c713005a2baaae3e69b9c30761eded56768e1c991c172910f
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: c7cb3c27bab85240bdb107f991a9d6d65145d083f432a499d81d1bfbc6384f84
Current Blockchain Height: 34
-----------------------------
Miner 3 added block 7
------ New Block ------ 
Parent Hash: c7cb3c27bab85240bdb107f991a9d6d65145d083f432a499d81d1bfbc6384f84
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: a4efcfd013cf58b65ef63168bab9d28cb6a63068147f756ba945fd706b0d9d8b
Current Blockchain Height: 35
-----------------------------
Miner 4 added block 7
------ New Block ------ 
Parent Hash: a4efcfd013cf58b65ef63168bab9d28cb6a63068147f756ba945fd706b0d9d8b
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: a115b2f1c7773f0ec24df7b63070b61db3e261132a2bcfa3e24b8e54590a58b6
Current Blockchain Height: 36
-----------------------------
Miner 4 added block 8
------ New Block ------ 
Parent Hash: a115b2f1c7773f0ec24df7b63070b61db3e261132a2bcfa3e24b8e54590a58b6
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 89681ba4c072b24edd572d29e799c88a19f6131352094e764024418b39d2fac0
Current Blockchain Height: 37
-----------------------------
Miner 1 added block 8
------ New Block ------ 
Parent Hash: 89681ba4c072b24edd572d29e799c88a19f6131352094e764024418b39d2fac0
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 41468facc4eb1642a0f078646445ba1d4f5ae96390f8f2db25251c43da483423
Current Blockchain Height: 38
-----------------------------
Miner 0 added block 9
------ New Block ------ 
Parent Hash: 41468facc4eb1642a0f078646445ba1d4f5ae96390f8f2db25251c43da483423
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 580a83809d9e23039f9a9eec059a7306461b64c7e4dfbce65f494991ac242d15
Current Blockchain Height: 39
-----------------------------
Miner 3 added block 8
------ New Block ------ 
Parent Hash: 580a83809d9e23039f9a9eec059a7306461b64c7e4dfbce65f494991ac242d15
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 90725a83585311e4cf466cdc60149108a2633e15b136bf5ac859e8e9251bfaaf
Current Blockchain Height: 40
-----------------------------
Miner 2 added block 7
------ New Block ------ 
Parent Hash: 90725a83585311e4cf466cdc60149108a2633e15b136bf5ac859e8e9251bfaaf
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 32b2b2d7180a40984e2a05c4f4bfa1b26e994c9577a203ce121936f7c04362df
Current Blockchain Height: 41
-----------------------------
Miner 1 added block 9
------ New Block ------ 
Parent Hash: 32b2b2d7180a40984e2a05c4f4bfa1b26e994c9577a203ce121936f7c04362df
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778756
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 784d6c4337eef29e384f8cd981272e19cf4c5d642b45ffe79424d19c7073621f
Current Blockchain Height: 42
-----------------------------
Miner 3 added block 9
------ New Block ------ 
Parent Hash: 784d6c4337eef29e384f8cd981272e19cf4c5d642b45ffe79424d19c7073621f
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778757
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 7af5f081f3fee9c7d7a53baefae7d26a1bd9b884cd5dff22e71f24d63738f101
Current Blockchain Height: 43
-----------------------------
Miner 4 added block 9
------ New Block ------ 
Parent Hash: 7af5f081f3fee9c7d7a53baefae7d26a1bd9b884cd5dff22e71f24d63738f101
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778757
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: fd2bff660f6e45486908846eb8f26d15ff0000a0ba1e9c575d56d286210d04a1
Current Blockchain Height: 44
-----------------------------
Miner 2 added block 8
------ New Block ------ 
Parent Hash: fd2bff660f6e45486908846eb8f26d15ff0000a0ba1e9c575d56d286210d04a1
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778757
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: f6b1cf49a5b56e1928cc870c4c7095d3dab01dd44e8d1f633ba6c4c55bee29b7
Current Blockchain Height: 45
-----------------------------
Miner 0 added block 10
------ New Block ------ 
Parent Hash: f6b1cf49a5b56e1928cc870c4c7095d3dab01dd44e8d1f633ba6c4c55bee29b7
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778757
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 838829886780ff21ed6e3776f5701ebe0818f96203d72cf1d98352e2491f2427
Current Blockchain Height: 46
-----------------------------
Miner 3 added block 10
------ New Block ------ 
Parent Hash: 838829886780ff21ed6e3776f5701ebe0818f96203d72cf1d98352e2491f2427
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778757
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 5b8c42d2d9c7012e6075697513b9bebe8d9323b2165c1646fcc13486b19bb793
Current Blockchain Height: 47
-----------------------------
Miner 2 added block 9
------ New Block ------ 
Parent Hash: 5b8c42d2d9c7012e6075697513b9bebe8d9323b2165c1646fcc13486b19bb793
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778757
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: d8dbee5e9d2729951b684010d4c3f816c7c31e422db6afa8fa5bfe7e1a302927
Current Blockchain Height: 48
-----------------------------
Miner 1 added block 10
------ New Block ------ 
Parent Hash: d8dbee5e9d2729951b684010d4c3f816c7c31e422db6afa8fa5bfe7e1a302927
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778757
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: a04b56dedfe8b02376fcf5790fb4ebf47a004dbee4727c1582318ab65e236725
Current Blockchain Height: 49
-----------------------------
Miner 4 added block 10
------ New Block ------ 
Parent Hash: a04b56dedfe8b02376fcf5790fb4ebf47a004dbee4727c1582318ab65e236725
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778757
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 7be6c6152b787b48f1b860cb217dfca5b458314d4e99b50c37aad144aa30355b
Current Blockchain Height: 50
-----------------------------
Miner 2 added block 10

Final Blockchain Details:
Current Blockchain Tip: 7be6c6152b787b48f1b860cb217dfca5b458314d4e99b50c37aad144aa30355b
Blockchain Size: 51
Blockchain Hashes:
d8dbee5e9d2729951b684010d4c3f816c7c31e422db6afa8fa5bfe7e1a302927
838829886780ff21ed6e3776f5701ebe0818f96203d72cf1d98352e2491f2427
f6b1cf49a5b56e1928cc870c4c7095d3dab01dd44e8d1f633ba6c4c55bee29b7
784d6c4337eef29e384f8cd981272e19cf4c5d642b45ffe79424d19c7073621f
7be6c6152b787b48f1b860cb217dfca5b458314d4e99b50c37aad144aa30355b
580a83809d9e23039f9a9eec059a7306461b64c7e4dfbce65f494991ac242d15
89681ba4c072b24edd572d29e799c88a19f6131352094e764024418b39d2fac0
a115b2f1c7773f0ec24df7b63070b61db3e261132a2bcfa3e24b8e54590a58b6
a4efcfd013cf58b65ef63168bab9d28cb6a63068147f756ba945fd706b0d9d8b
a04b56dedfe8b02376fcf5790fb4ebf47a004dbee4727c1582318ab65e236725
e1a4d0c9d4abec081fc10222bf4ee5360ceba53dc596c95e223cab91072c7857
cd49350b7ffd69a6cd43d5d5f2e12bcd6d0c8f57fed3f5140754de936f85ada2
f5317ffdfcabbb50e87932d425ed8d3345ad23ee8b2689dc2dfe480a071b59b7
652856549f78ab159e33051b62ec710948ae5c794edc0fbe24aab315e7ff9122
888203f3475d49830605b34ecdaadcac5bda28effeb0d438035e32ae8748387b
fd2bff660f6e45486908846eb8f26d15ff0000a0ba1e9c575d56d286210d04a1
71f26ef996c2e37853c15e1b817b986cbc1c6b19954b33983086f4f519af014d
7ee95742c86c5725eec60206e2ffa8861ff70cedb4549dc1905367c2c31fc5ee
cf6b92d56d2c8fa61e342979c57ee7d512744f5585da51438f82cee9a29d260a
4ffbcc03cbecaa71cd565d46aa792ff4f0ca3a7e174e7175b84570c1b4c4198f
32b2b2d7180a40984e2a05c4f4bfa1b26e994c9577a203ce121936f7c04362df
5c59f7c1b362e57e21e752fe0ceb9a27673a8eb3aa2973b5e143752465716166
640444babb57db29b765d16006309ac2ed0ea1be8286eafac44957a97db31ffb
7af5f081f3fee9c7d7a53baefae7d26a1bd9b884cd5dff22e71f24d63738f101
3148926bf46b5410c9b51eb1faa481bbf930dc92b9542f779749dc67aae2ab0e
50dccf0c204371ba8275b0c3433f5a4af2316ed742b915e85894bc6f48b74031
96c72b37626a66c94a0a8120917189e9563f1d17ad56f23ad05693de0d0489dc
7ef1f9d661e3e7cdb30c9cd95d20faa926a136355f6e3e370da2711936416010
9e0f95a12b67c059c9f242a8ae4f9ff331cbd968b50990879fcdf1c17b3ac6cc
b2d81b94c3f1dcdb23ee4fbcfdb43aa5b25f29effa7d8b0563fe8bdbff22e01b
5b8c42d2d9c7012e6075697513b9bebe8d9323b2165c1646fcc13486b19bb793
41468facc4eb1642a0f078646445ba1d4f5ae96390f8f2db25251c43da483423
8f8f70e4d418d40bb6a3b9463ca093bb0e1fb74b5077f0efe97d28358e26f348
315f5c2b7d434f6c713005a2baaae3e69b9c30761eded56768e1c991c172910f
474a61ee0fb681b1436cd3fbd234c38694d729926ba1b415d27cee1ac04abde2
b01fb95db56cd4f4380c2531a782773f731fd7481eee011a927babf523aab7ee
808b16ea7167518d6624229bb4235ca0e127b94320782baab3529d3bfb80fac6
a14d816b96e0d432e8d567bf37f0095ccc50a164e9a8f50a0c249603b3e1ff32
bee3d35ad917d46d246a65a735c4ef17693cce9a9cdc552e007a18f93e022228
0ad75d7cabc7e93e929fa6dafd8f13019504e3c6c8d8c3042cc90a0a3f513b38
90725a83585311e4cf466cdc60149108a2633e15b136bf5ac859e8e9251bfaaf
a891392c95c8278629ccd58a719d6d33d2c97eaa1163260e32b7fb43f7a44e45
0bb538f4d735bbc8bc7c2c0089577aec2ad4aa4c821913148df7f42929eafa37
f02c8585c05b9be00ab2936c2f9de1e82f30af34f556ba9b8c25afee7a1bd678
717ba83145c4140b055cf4780eb47a95614e7f9ca9f04c198ae18033ffca6e88
c62974548342118891e714ffc1cca7f3a5674ad47b9f64e262a4092be35a6b55
c7cb3c27bab85240bdb107f991a9d6d65145d083f432a499d81d1bfbc6384f84
eb4dbb7bf73274bbcccf980788956dcecee6018a5e10c757d33d139499ee9c50
635fb61b6e3bac2c072c5f726a9b37c378a5922f1cc0670ad07e8935528c6155
49d0db1daf4d8a5ddf63965c4ad4f98369f686ee80488b22723dd85e975631f2
19ca652cb5e12abd26c5e5000aa56541b3d6ffd41d7083d23472eb789135e42f

*/
