#include "gtstore.hpp"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <time.h>

#define PASS "\033[32;1m PASS \033[0m\n"
#define FAIL "\033[31;1m FAIL \033[0m\n"

void test_data_partitioning(int* pids) {
    cout << "================== Get key distribution ==================\n";
    GTStoreClient client;
    client.init(0);
    srand(time(NULL));
    for (int i = 0; i < 1000; i ++) {
        string key = "";
        int len = rand() % 20 + 10;
        for( int j = 0; j < len; ++j){
            key = key + "?";
            key[j] = 'a' + rand()%26;
        }
        cout << key << endl;
        vector<string> value;
        value.push_back(key);
        client.put(key, value);
    }
    client.finalize();
    sleep(5);

    std::ofstream outfile;
    outfile.open("key.distribution.txt");
    for(unsigned int i = 0; i < 10; i++) {
        int pid = pids[i];
        kill(pid, SIGUSR1);
        sleep(1);
        std::ifstream infile;
        infile.open("/tmp/storage.stats.txt");
        int num_keys;
        while (infile >> num_keys)
        {
            cout << "count " << pid << ":" << num_keys << endl;
            outfile << std::to_string(num_keys) << endl;
        }
        infile.close();
    }
    outfile.close();
    cout << "Check key.distribution.txt for results" << endl;
}
void test_put_get_faulty_node(int faulty_node_pid) {
    cout << "================== Test PUT GET with faulty node ==================\n";

    GTStoreClient client;
    client.init(0);

    string key = "animals";
    vector<string> value;
    value.push_back("dog");
    value.push_back("cat");

    client.put(key, value);
    cout << PASS;

    kill(faulty_node_pid, SIGTERM);
    val_t res = client.get(key);
    auto val = res.begin();

    if (string(*val) != "dog") {
        cout << FAIL;
        client.finalize();
        return;
    }

    val++;
    if (string(*val) != "cat") {
        cout << FAIL;
        client.finalize();
        return;
    }

    cout << PASS;
    client.finalize();
}
void test_basic_put_get() {
    cout << "================== Test Basic PUT GET ==================\n";

    GTStoreClient client;
    client.init(0);

    string key = "animals";
    vector<string> value;
    value.push_back("dog");
    value.push_back("cat");

    client.put(key, value);
    cout << PASS;

    val_t res = client.get(key);
    auto val = res.begin();

    if (string(*val) != "dog") {
        cout << FAIL;
        client.finalize();
        return;
    }

    val++;
    if (string(*val) != "cat") {
        cout << FAIL;
        client.finalize();
        return;
    }

    cout << PASS;
    client.finalize();
}

int main(int argc, char **argv) {
	string test = string(argv[1]);
	if (string(argv[1]) == "basic_put_get") {
		test_basic_put_get();
	} else if (string(argv[1]) == "put_get_faulty_node") {
	    int pid = atoi(argv[2]);
        test_put_get_faulty_node(pid);
    } else if (string(argv[1]) == "data_partitioning") {
	    int* pids = new int[10] {};
	    for(int i = 0; i < 10; i++) {
	        pids[i] = atoi(argv[2+i]);

	    }
        test_data_partitioning(pids);
    }
}
