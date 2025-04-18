#include <algorithm>
#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <map>
#include <sys/select.h>
#include <sys/types.h>
#include <vector>
#include <unistd.h>

class SOCDBind {
public:
	std::vector<uint16_t> keysPrio = std::vector<uint16_t>();
	std::vector<uint16_t> keys = std::vector<uint16_t>();
	//Keep track of fake key states to only release the necessary ones to make detection harder
	std::map<uint16_t, bool> keyStates = std::map<uint16_t, bool>();

	SOCDBind(std::vector<uint16_t> keys) : keys(keys) { }

	bool affects(uint16_t key)
	{
		return std::find(keys.begin(), keys.end(), key) != keys.end();
	}

	void setKeyDown(uint16_t key, bool down, bool real)
	{
		if (!affects(key))
			return;

		if (!real)
		{
			keyStates[key] = down;
			return;
		}

		auto idx = std::find(keysPrio.begin(), keysPrio.end(), key);
		if (down && idx == keysPrio.end()) //Prevent repeat spam
		{
			keysPrio.emplace_back(key);
		}
		else if (!down && idx != keysPrio.end())
		{
			keysPrio.erase(idx);
		}
	}

	bool shouldRelease(uint16_t key)
	{
		if (!affects(key))
			return false;

		return keyStates.count(key) > 0 && keyStates.at(key);
	}
};

bool readEvent(input_event& event)
{
	return fread(&event, sizeof(struct input_event), 1, stdin) != 0;
}

void writeEvent(input_event& event)
{
	fwrite(&event, sizeof(struct input_event), 1, stdout);
}

void writeEvent(uint16_t type, uint16_t code, uint32_t val)
{
	static input_event event {};
	event.type = type;
	event.code = code;
	event.value = val;

	writeEvent(event);
}

static SOCDBind binds[] =
{
	SOCDBind({ KEY_A, KEY_D }),
	SOCDBind({ KEY_W, KEY_S }),

	SOCDBind({ KEY_LEFT, KEY_RIGHT }),
	SOCDBind({ KEY_UP, KEY_DOWN }),

	//Vim style
	SOCDBind({ KEY_H, KEY_L }),
	SOCDBind({ KEY_J, KEY_K }),
	
	//Binding of isaac
	SOCDBind({ KEY_NUMERIC_4, KEY_NUMERIC_5, KEY_NUMERIC_6, KEY_NUMERIC_8 }),
};

int main(int numArgs, const char** ppChArgs)
{
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	input_event event{};
	while(readEvent(event))
	{
		if (event.type != EV_KEY)
		{
			writeEvent(event);
			continue;
		}

		bool write = true;

		for (auto& socd : binds)
		{
			if (!socd.affects(event.code))
			{
				continue;
			}

			socd.setKeyDown(event.code, event.value > 0, true);

			if (socd.keysPrio.empty())
			{
				continue;
			}

			uint16_t prio = socd.keysPrio.back();
			for (auto& key : socd.keys)
			{
				uint32_t val;
				if (key == prio)
				{
					val = 1;
				}
				else if (socd.shouldRelease(key))
				{
					val = 0;
				}
				else
				{
					continue;
				}

				writeEvent(EV_KEY, key, val);
				socd.setKeyDown(key, val, false);
			}

			write = false;
		}

		if (write)
		{
			writeEvent(event);
		}
	}

	return 0;
}
