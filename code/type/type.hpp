#include <string>
#include <vector>
#include <map>
#include <cstdint>


using namespace std;

typedef vector<string> val_t;

typedef struct {
	int32_t vectorClock;
	val_t value;
	bool empty;
} final_val_t;


typedef vector<string> PrefListType;
typedef map<size_t, string> HashMapType;
