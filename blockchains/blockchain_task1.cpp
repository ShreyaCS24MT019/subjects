#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <iomanip>
#include <sstream>
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

public:
    Blockchain() {
        Block genesisBlock;
        cout << "Genesis block created with hash: " << genesisBlock.hash << endl;
        blocks[genesisBlock.hash] = genesisBlock;
        tip = genesisBlock.hash;
    }

    void addBlock() {
        Block newBlock(tip);
       
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
};

int main() {
    Blockchain blockchain;

    for (int i = 0; i < 50; ++i) {
        blockchain.addBlock();
    }

    cout << "Current Blockchain Tip: " << blockchain.getTip() << endl;
    blockchain.displayBlockchainHashes();

    return 0;
    
}

/* OUTPUT 


./blockchain_task1
Genesis block created with hash: 3ee35763a63ab2c388c7e78afa8093478ffa1bb1d497fbb49d831d1896a5dd40
------ New Block ------ 
Parent Hash: 3ee35763a63ab2c388c7e78afa8093478ffa1bb1d497fbb49d831d1896a5dd40
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: fcb89547d6a9a9112ea99b489ef74e132baded6004c9820ba3c3a08e54f29d05
Current Blockchain Height: 1
-----------------------------
------ New Block ------ 
Parent Hash: fcb89547d6a9a9112ea99b489ef74e132baded6004c9820ba3c3a08e54f29d05
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 1dad05f513ea75ead2685c22b2912239e7e7c39a64075f959c222ca2ffa2cd48
Current Blockchain Height: 2
-----------------------------
------ New Block ------ 
Parent Hash: 1dad05f513ea75ead2685c22b2912239e7e7c39a64075f959c222ca2ffa2cd48
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 2f644242eef3b7ac465b7fd3c435d8a24b4a03ebdae6b0fd2f0358de5f6d03b4
Current Blockchain Height: 3
-----------------------------
------ New Block ------ 
Parent Hash: 2f644242eef3b7ac465b7fd3c435d8a24b4a03ebdae6b0fd2f0358de5f6d03b4
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 348cf9a94c14930a5aebfc73dfeda6a352efe24c1bf67b4a87e9fb0038f62f37
Current Blockchain Height: 4
-----------------------------
------ New Block ------ 
Parent Hash: 348cf9a94c14930a5aebfc73dfeda6a352efe24c1bf67b4a87e9fb0038f62f37
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: be6a2b32eaeec01093a2781ba451e51b9e27c0c07ac11121348ffecb5ac7bca2
Current Blockchain Height: 5
-----------------------------
------ New Block ------ 
Parent Hash: be6a2b32eaeec01093a2781ba451e51b9e27c0c07ac11121348ffecb5ac7bca2
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 2150ce75af05e1fc806095a5b84086b385eaf6388723b32972e91d2dca1a4c3b
Current Blockchain Height: 6
-----------------------------
------ New Block ------ 
Parent Hash: 2150ce75af05e1fc806095a5b84086b385eaf6388723b32972e91d2dca1a4c3b
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: dd83771218c1f54be7215d3e2285ad608762f4e45c063dc4f3a1518692e24288
Current Blockchain Height: 7
-----------------------------
------ New Block ------ 
Parent Hash: dd83771218c1f54be7215d3e2285ad608762f4e45c063dc4f3a1518692e24288
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: b7a83da2f7fb3096c6756bca399ce8da1d73e33ae75df2829a0442586428f4ef
Current Blockchain Height: 8
-----------------------------
------ New Block ------ 
Parent Hash: b7a83da2f7fb3096c6756bca399ce8da1d73e33ae75df2829a0442586428f4ef
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: c0804d0fe6afb469d763909d01f70a3b320f55eec0065c0842e3c7f2bfbb358b
Current Blockchain Height: 9
-----------------------------
------ New Block ------ 
Parent Hash: c0804d0fe6afb469d763909d01f70a3b320f55eec0065c0842e3c7f2bfbb358b
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 04465af422f1252d3b4dd295ab4c2be46ae1bfaad22223885ccd6541158e77a1
Current Blockchain Height: 10
-----------------------------
------ New Block ------ 
Parent Hash: 04465af422f1252d3b4dd295ab4c2be46ae1bfaad22223885ccd6541158e77a1
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 96ebc3fd922f5ba4ab87f22de02af6898a646e9c6d0480ccc18972c4350854bd
Current Blockchain Height: 11
-----------------------------
------ New Block ------ 
Parent Hash: 96ebc3fd922f5ba4ab87f22de02af6898a646e9c6d0480ccc18972c4350854bd
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 69d10d2cccf91070b780ab56369d0843fe536caffb4d4378f8b078865ecefc4a
Current Blockchain Height: 12
-----------------------------
------ New Block ------ 
Parent Hash: 69d10d2cccf91070b780ab56369d0843fe536caffb4d4378f8b078865ecefc4a
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: ef1feb306a7c58d3e3b7c14d27aa38d5da7ba026a4cc7e93d15e81b0fd09b1f5
Current Blockchain Height: 13
-----------------------------
------ New Block ------ 
Parent Hash: ef1feb306a7c58d3e3b7c14d27aa38d5da7ba026a4cc7e93d15e81b0fd09b1f5
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: de4513cd33b5b6bb2320e49890994bea602fe8d272c6cfde9536ba48b12b8979
Current Blockchain Height: 14
-----------------------------
------ New Block ------ 
Parent Hash: de4513cd33b5b6bb2320e49890994bea602fe8d272c6cfde9536ba48b12b8979
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 9bc7e89762075032a1c2676bd9bfc386576539a17a07bebc740e7f4c06af1f80
Current Blockchain Height: 15
-----------------------------
------ New Block ------ 
Parent Hash: 9bc7e89762075032a1c2676bd9bfc386576539a17a07bebc740e7f4c06af1f80
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 96e0cb6073b5260d5546f1717a2f0cd58b437ee38d3b812bb2e0b81ee76ce8e0
Current Blockchain Height: 16
-----------------------------
------ New Block ------ 
Parent Hash: 96e0cb6073b5260d5546f1717a2f0cd58b437ee38d3b812bb2e0b81ee76ce8e0
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 012113c39ac2919df097be57b8c6db9a00ad4fa2568040fa67e3faed31e966d0
Current Blockchain Height: 17
-----------------------------
------ New Block ------ 
Parent Hash: 012113c39ac2919df097be57b8c6db9a00ad4fa2568040fa67e3faed31e966d0
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 9f1bf22a4dfba30c09c70c17e311cab6fff296c3db22b199b3f93779ee07a398
Current Blockchain Height: 18
-----------------------------
------ New Block ------ 
Parent Hash: 9f1bf22a4dfba30c09c70c17e311cab6fff296c3db22b199b3f93779ee07a398
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: ef7c510a5c191054ef1a40ce3bacd9accd021f4ed9c10d83bb3f321905c60a9f
Current Blockchain Height: 19
-----------------------------
------ New Block ------ 
Parent Hash: ef7c510a5c191054ef1a40ce3bacd9accd021f4ed9c10d83bb3f321905c60a9f
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 559674bd8bb3b01c9cba8112453d17b2ac2e6a1b00b6a55709e158d0dae4f576
Current Blockchain Height: 20
-----------------------------
------ New Block ------ 
Parent Hash: 559674bd8bb3b01c9cba8112453d17b2ac2e6a1b00b6a55709e158d0dae4f576
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: f55f8291b6d4632f60a5800964c5692b74277dbc3104520a212aab1347f62686
Current Blockchain Height: 21
-----------------------------
------ New Block ------ 
Parent Hash: f55f8291b6d4632f60a5800964c5692b74277dbc3104520a212aab1347f62686
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 77e6b24e0a2e5171cc4105276c6cee98167f1b280efedb43f037c16e65ef705b
Current Blockchain Height: 22
-----------------------------
------ New Block ------ 
Parent Hash: 77e6b24e0a2e5171cc4105276c6cee98167f1b280efedb43f037c16e65ef705b
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: ad97eeb091b266eb1fc41439c5dbec5091d4cf7a75a4d536481eab51f2dad1a6
Current Blockchain Height: 23
-----------------------------
------ New Block ------ 
Parent Hash: ad97eeb091b266eb1fc41439c5dbec5091d4cf7a75a4d536481eab51f2dad1a6
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 6016bce567024467680e12445a857568fc636f9b5b3fbb57d6842c0118095996
Current Blockchain Height: 24
-----------------------------
------ New Block ------ 
Parent Hash: 6016bce567024467680e12445a857568fc636f9b5b3fbb57d6842c0118095996
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 77efc6a2e437e900554798a25e6bacd48f9afa662a563f8c2c642134654dce2f
Current Blockchain Height: 25
-----------------------------
------ New Block ------ 
Parent Hash: 77efc6a2e437e900554798a25e6bacd48f9afa662a563f8c2c642134654dce2f
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 1778cf300c988e5444a40fddd32e5963f18e70c5823437ab18d583bc2539ed21
Current Blockchain Height: 26
-----------------------------
------ New Block ------ 
Parent Hash: 1778cf300c988e5444a40fddd32e5963f18e70c5823437ab18d583bc2539ed21
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 4dc418849fd51a3d4e731774da164ba790b93a30de179492cc4df8dc99d092ee
Current Blockchain Height: 27
-----------------------------
------ New Block ------ 
Parent Hash: 4dc418849fd51a3d4e731774da164ba790b93a30de179492cc4df8dc99d092ee
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 7332d477e874517b4e9914b4f2f51f05268ad5dfba107af4982b3703f6c1fab0
Current Blockchain Height: 28
-----------------------------
------ New Block ------ 
Parent Hash: 7332d477e874517b4e9914b4f2f51f05268ad5dfba107af4982b3703f6c1fab0
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 827740ba2bd3dd2de0c7f3cc86c184d81daf2c98a5ca95a7eab5aeab25ff7069
Current Blockchain Height: 29
-----------------------------
------ New Block ------ 
Parent Hash: 827740ba2bd3dd2de0c7f3cc86c184d81daf2c98a5ca95a7eab5aeab25ff7069
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 91ff2ad6515f69cb84a5761e4c756b7b6288e581a750dbdfebeac3801c5fb3f4
Current Blockchain Height: 30
-----------------------------
------ New Block ------ 
Parent Hash: 91ff2ad6515f69cb84a5761e4c756b7b6288e581a750dbdfebeac3801c5fb3f4
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 32e1f23ea0e09b0dbec783e679201bf4a61379f78c7b4deac306070a7e141846
Current Blockchain Height: 31
-----------------------------
------ New Block ------ 
Parent Hash: 32e1f23ea0e09b0dbec783e679201bf4a61379f78c7b4deac306070a7e141846
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 3808fa56b03cf1a32d7398a438aedcf234086b3407fe3fa48260663ee389b09a
Current Blockchain Height: 32
-----------------------------
------ New Block ------ 
Parent Hash: 3808fa56b03cf1a32d7398a438aedcf234086b3407fe3fa48260663ee389b09a
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 158b88a02e08dc9ca6fb339964a9424691442196ba48e6c4b3fb435578cbc73c
Current Blockchain Height: 33
-----------------------------
------ New Block ------ 
Parent Hash: 158b88a02e08dc9ca6fb339964a9424691442196ba48e6c4b3fb435578cbc73c
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 15be781b19ed429598ad3a9400c49404378a986f216a2606b62b0a7a2581d0fc
Current Blockchain Height: 34
-----------------------------
------ New Block ------ 
Parent Hash: 15be781b19ed429598ad3a9400c49404378a986f216a2606b62b0a7a2581d0fc
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: c50c6cbcffe2e37d7d384b6799f6ba8c18a2f92cb40a8d219592878eecc6f576
Current Blockchain Height: 35
-----------------------------
------ New Block ------ 
Parent Hash: c50c6cbcffe2e37d7d384b6799f6ba8c18a2f92cb40a8d219592878eecc6f576
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: e06ed21af95ace263c9985076147d205d29e6dd3f0a43e3e903d9c8cd72cb9e3
Current Blockchain Height: 36
-----------------------------
------ New Block ------ 
Parent Hash: e06ed21af95ace263c9985076147d205d29e6dd3f0a43e3e903d9c8cd72cb9e3
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: c0ffebfb1647e2c144d6dfc10cd847fca9a26e64bbcf009577cce425885a65b1
Current Blockchain Height: 37
-----------------------------
------ New Block ------ 
Parent Hash: c0ffebfb1647e2c144d6dfc10cd847fca9a26e64bbcf009577cce425885a65b1
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: ab2a4872f1788713b8966ea3676d536799061547192d13301e43552382f61e1b
Current Blockchain Height: 38
-----------------------------
------ New Block ------ 
Parent Hash: ab2a4872f1788713b8966ea3676d536799061547192d13301e43552382f61e1b
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 95fc6846d345394ea5b7e1e1bf64fe94ca80b5cfd7a37a24c5c5e04ccc761dc8
Current Blockchain Height: 39
-----------------------------
------ New Block ------ 
Parent Hash: 95fc6846d345394ea5b7e1e1bf64fe94ca80b5cfd7a37a24c5c5e04ccc761dc8
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 44e3cef3619540d6de2337942379cfe7074fa38462608e4de922c520a1fb217b
Current Blockchain Height: 40
-----------------------------
------ New Block ------ 
Parent Hash: 44e3cef3619540d6de2337942379cfe7074fa38462608e4de922c520a1fb217b
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 7b24c981c7941e1eb09f6e651655dc9d3899266b90cbdd23e9df83b0815ff5f8
Current Blockchain Height: 41
-----------------------------
------ New Block ------ 
Parent Hash: 7b24c981c7941e1eb09f6e651655dc9d3899266b90cbdd23e9df83b0815ff5f8
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 0dace89b026e3f8c0ae126e67508a1db437190a00274ee5c8cb18d3e8fbbfb1e
Current Blockchain Height: 42
-----------------------------
------ New Block ------ 
Parent Hash: 0dace89b026e3f8c0ae126e67508a1db437190a00274ee5c8cb18d3e8fbbfb1e
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 1988e715cebe199c93e0145e0d9d1d9cec032e4b9292f836166f072a764ad601
Current Blockchain Height: 43
-----------------------------
------ New Block ------ 
Parent Hash: 1988e715cebe199c93e0145e0d9d1d9cec032e4b9292f836166f072a764ad601
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: c10751d038405dc1c5ed2991be8ad3eb57cc8d76b6bf8179c4eb23dfbb205f48
Current Blockchain Height: 44
-----------------------------
------ New Block ------ 
Parent Hash: c10751d038405dc1c5ed2991be8ad3eb57cc8d76b6bf8179c4eb23dfbb205f48
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 5f97789721c5165e2649c61c971c6a29da018794659543054767a324befc912a
Current Blockchain Height: 45
-----------------------------
------ New Block ------ 
Parent Hash: 5f97789721c5165e2649c61c971c6a29da018794659543054767a324befc912a
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 2e08dbd39b24a28d7cedbe050fa3b61f235c04cdf877c281fa20107c73e98dba
Current Blockchain Height: 46
-----------------------------
------ New Block ------ 
Parent Hash: 2e08dbd39b24a28d7cedbe050fa3b61f235c04cdf877c281fa20107c73e98dba
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: e8e84a25a1cfe067f90f709668257577ad4fd23fd4f8013c31b65d5273438be2
Current Blockchain Height: 47
-----------------------------
------ New Block ------ 
Parent Hash: e8e84a25a1cfe067f90f709668257577ad4fd23fd4f8013c31b65d5273438be2
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 6c3241cff433babad95fee2f0f3f3d60658c5ad52a498e5e7c7be1f0283aa9a5
Current Blockchain Height: 48
-----------------------------
------ New Block ------ 
Parent Hash: 6c3241cff433babad95fee2f0f3f3d60658c5ad52a498e5e7c7be1f0283aa9a5
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: ab77d244457dd74486fec9e27c70c6636491e03c8fe59f2dade71b84fde1a2d3
Current Blockchain Height: 49
-----------------------------
------ New Block ------ 
Parent Hash: ab77d244457dd74486fec9e27c70c6636491e03c8fe59f2dade71b84fde1a2d3
Nonce: 1000
Difficulty: 0000000000000000000000000000000000000000
Timestamp: 1738778745
Merkle Root: 82d5dc2f0752fb74781033d84752bb705458e9c6321eb48322874fb0ae82f85b
Hash: 65a139f317ab20df1ebbac6647ec963a822736c7682cba99c28f1c16e3b44e3d
Current Blockchain Height: 50
-----------------------------
Current Blockchain Tip: 65a139f317ab20df1ebbac6647ec963a822736c7682cba99c28f1c16e3b44e3d
Blockchain Hashes:
6c3241cff433babad95fee2f0f3f3d60658c5ad52a498e5e7c7be1f0283aa9a5
e8e84a25a1cfe067f90f709668257577ad4fd23fd4f8013c31b65d5273438be2
5f97789721c5165e2649c61c971c6a29da018794659543054767a324befc912a
1988e715cebe199c93e0145e0d9d1d9cec032e4b9292f836166f072a764ad601
0dace89b026e3f8c0ae126e67508a1db437190a00274ee5c8cb18d3e8fbbfb1e
7b24c981c7941e1eb09f6e651655dc9d3899266b90cbdd23e9df83b0815ff5f8
e06ed21af95ace263c9985076147d205d29e6dd3f0a43e3e903d9c8cd72cb9e3
c50c6cbcffe2e37d7d384b6799f6ba8c18a2f92cb40a8d219592878eecc6f576
15be781b19ed429598ad3a9400c49404378a986f216a2606b62b0a7a2581d0fc
65a139f317ab20df1ebbac6647ec963a822736c7682cba99c28f1c16e3b44e3d
32e1f23ea0e09b0dbec783e679201bf4a61379f78c7b4deac306070a7e141846
91ff2ad6515f69cb84a5761e4c756b7b6288e581a750dbdfebeac3801c5fb3f4
c10751d038405dc1c5ed2991be8ad3eb57cc8d76b6bf8179c4eb23dfbb205f48
96ebc3fd922f5ba4ab87f22de02af6898a646e9c6d0480ccc18972c4350854bd
ab2a4872f1788713b8966ea3676d536799061547192d13301e43552382f61e1b
fcb89547d6a9a9112ea99b489ef74e132baded6004c9820ba3c3a08e54f29d05
c0ffebfb1647e2c144d6dfc10cd847fca9a26e64bbcf009577cce425885a65b1
b7a83da2f7fb3096c6756bca399ce8da1d73e33ae75df2829a0442586428f4ef
c0804d0fe6afb469d763909d01f70a3b320f55eec0065c0842e3c7f2bfbb358b
827740ba2bd3dd2de0c7f3cc86c184d81daf2c98a5ca95a7eab5aeab25ff7069
ad97eeb091b266eb1fc41439c5dbec5091d4cf7a75a4d536481eab51f2dad1a6
44e3cef3619540d6de2337942379cfe7074fa38462608e4de922c520a1fb217b
6016bce567024467680e12445a857568fc636f9b5b3fbb57d6842c0118095996
ab77d244457dd74486fec9e27c70c6636491e03c8fe59f2dade71b84fde1a2d3
2150ce75af05e1fc806095a5b84086b385eaf6388723b32972e91d2dca1a4c3b
dd83771218c1f54be7215d3e2285ad608762f4e45c063dc4f3a1518692e24288
012113c39ac2919df097be57b8c6db9a00ad4fa2568040fa67e3faed31e966d0
3808fa56b03cf1a32d7398a438aedcf234086b3407fe3fa48260663ee389b09a
559674bd8bb3b01c9cba8112453d17b2ac2e6a1b00b6a55709e158d0dae4f576
348cf9a94c14930a5aebfc73dfeda6a352efe24c1bf67b4a87e9fb0038f62f37
4dc418849fd51a3d4e731774da164ba790b93a30de179492cc4df8dc99d092ee
be6a2b32eaeec01093a2781ba451e51b9e27c0c07ac11121348ffecb5ac7bca2
1dad05f513ea75ead2685c22b2912239e7e7c39a64075f959c222ca2ffa2cd48
69d10d2cccf91070b780ab56369d0843fe536caffb4d4378f8b078865ecefc4a
2f644242eef3b7ac465b7fd3c435d8a24b4a03ebdae6b0fd2f0358de5f6d03b4
9f1bf22a4dfba30c09c70c17e311cab6fff296c3db22b199b3f93779ee07a398
ef7c510a5c191054ef1a40ce3bacd9accd021f4ed9c10d83bb3f321905c60a9f
ef1feb306a7c58d3e3b7c14d27aa38d5da7ba026a4cc7e93d15e81b0fd09b1f5
7332d477e874517b4e9914b4f2f51f05268ad5dfba107af4982b3703f6c1fab0
9bc7e89762075032a1c2676bd9bfc386576539a17a07bebc740e7f4c06af1f80
77efc6a2e437e900554798a25e6bacd48f9afa662a563f8c2c642134654dce2f
95fc6846d345394ea5b7e1e1bf64fe94ca80b5cfd7a37a24c5c5e04ccc761dc8
04465af422f1252d3b4dd295ab4c2be46ae1bfaad22223885ccd6541158e77a1
3ee35763a63ab2c388c7e78afa8093478ffa1bb1d497fbb49d831d1896a5dd40
96e0cb6073b5260d5546f1717a2f0cd58b437ee38d3b812bb2e0b81ee76ce8e0
158b88a02e08dc9ca6fb339964a9424691442196ba48e6c4b3fb435578cbc73c
f55f8291b6d4632f60a5800964c5692b74277dbc3104520a212aab1347f62686
1778cf300c988e5444a40fddd32e5963f18e70c5823437ab18d583bc2539ed21
2e08dbd39b24a28d7cedbe050fa3b61f235c04cdf877c281fa20107c73e98dba
de4513cd33b5b6bb2320e49890994bea602fe8d272c6cfde9536ba48b12b8979
77e6b24e0a2e5171cc4105276c6cee98167f1b280efedb43f037c16e65ef705b


*/
