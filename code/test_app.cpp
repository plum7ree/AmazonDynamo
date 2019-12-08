#include "gtstore.hpp"
#include <unistd.h>
#include <fstream>
#include <iostream>

#define PASS "\033[32;1m PASS \033[0m\n"
#define FAIL "\033[31;1m FAIL \033[0m\n"

void test_data_partitioning(int* pids) {
    cout << "================== Populate data for data_partitioning ==================\n";
    GTStoreClient client;
    client.init(0);
    for (int i = 0; i < 100; i ++) {
        string key = "somerandomkey" + std::to_string(i);
        vector<string> value;
        value.push_back("randomvalue");
        client.put(key, value);
    }
    client.finalize();
    sleep(10);

    for(unsigned int i = 0; i < 5; i++) {
        int pid = pids[i];
        kill(pid, SIGUSR1);
        usleep(1000000);
        std::ifstream infile;
        infile.open("/tmp/storage.stats.txt");
        int num_keys;
        while (infile >> num_keys)
        {
            cout << "count " << pid << ":" << num_keys << endl;
        }
    }
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
	    int* pids = new int[5] { atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]) };
        test_data_partitioning(pids);
    }
}
