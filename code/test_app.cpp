#include "gtstore.hpp"

#define PASS "\033[32;1m PASS \033[0m\n"
#define FAIL "\033[31;1m FAIL \033[0m\n"

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
	}
}
