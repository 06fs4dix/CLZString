/*
https://github.com/jawa-the-hutt/lz-string-csharp
C++ 버전이 없어서 c#버전을 수정 했다
위 url버전을 참고 했고, 나 처럼 고생하지 않길 바란다
버그가 많은데 수정하게 되면 알려달라!
00ktdaix@gmail.com

I did not have a C ++ version, so I modified the c # version.
I refer to the above url version, and I do not want you to suffer like me.
If you have a lot of bugs, let me know!
00ktdaix@gmail.com

The latest version was not able to transform it into my project ...
*/



#pragma once
//#include "CString.h"

#include <string>
#include <vector>
#include <set>
#include <map>
using std::set;
using std::map;
using std::string;
using std::wstring;
using std::vector;

struct dataStruct
{
	int val, position, index;
};
const static wstring keyStrBase64 = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
const static wstring keyStrUriSafe = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-$";
static map<wstring, map<char, int>> baseReverseDic;
static int getBaseValue(wstring alphabet, char character)
{
	if (baseReverseDic.find(alphabet) == baseReverseDic.end())
	{
		baseReverseDic[alphabet] = map<char, int>();
		for (int i = 0; i < alphabet.length(); i++)
		{
			baseReverseDic[alphabet][alphabet[i]] = i;
		}
	}
	return baseReverseDic[alphabet][character];
}
typedef		int(*GetNextValue)(wstring&,int);
typedef		wstring (*GetCharFromInt)(int);

int GetVal(wstring &compressed, int index)
{
	return compressed[index];
}
int GetBase64(wstring &compressed, int index)
{
	return getBaseValue(keyStrBase64, compressed[index]);
}
wstring KeyToBase64(int a)
{
	wstring dum;
	dum+= keyStrBase64[a];
	return dum;
}
static wstring f(int val)
{
	wstring dum;
	dum += wchar_t(val);
	return dum;
}
class CLZString
{
public:
	static wstring compressToBase64(wstring input)
	{
		
		wstring res = _compress(input, 6, KeyToBase64);
		switch (res.length() % 4)
		{
		case 0: return res;
		case 1: return res + L"===";
		case 2: return res + L"==";
		case 3: return res + L"=";
		}
		return wstring();
	}
	//static int getBaseValue();
	static wstring Decompres(wstring input)
	{
		return _decompress(input.length(), 32, GetVal, input);
	}
	static wstring DecompressBase64(wstring input)
	{
		return _decompress(input.length(), 32, GetBase64, input);
	}

	static wstring compress(wstring uncompressed)
	{
		return _compress(uncompressed, 16, f);
	}

	static wstring _compress(wstring uncompressed, int bitsPerChar, GetCharFromInt getCharFromInt)
	{
		int i, value, ii, context_enlargeIn = 2, context_dictSize = 3, context_numBits = 2, context_data_val = 0, context_data_position = 0;
		map<wstring, bool> context_mapToCreate;
		map<wstring, int> context_map;
		wstring context_data;
		wstring context_c;
		wstring context_wc, context_w;

		for (ii = 0; ii < uncompressed.length(); ii++)
		{
			context_c = uncompressed[ii];
			if (context_map.find(context_c)== context_map.end())
			{
				context_map[context_c] = context_dictSize++;
				context_mapToCreate[context_c] = true;
			}
			context_wc = context_w + context_c;
			if (context_map.find(context_wc)!= context_map.end())
			{
				context_w = context_wc;
			}
			else
			{
				if (context_mapToCreate.find(context_w)!= context_mapToCreate.end())
				{
					if (int(context_w[0]) < 256)
					{
						for (i = 0; i < context_numBits; i++)
						{
							context_data_val = (context_data_val << 1);
							if (context_data_position == bitsPerChar - 1)
							{
								context_data_position = 0;
								context_data.append(getCharFromInt(context_data_val));
								context_data_val = 0;
							}
							else
							{
								context_data_position++;
							}
						}
						value = int(context_w[0]);
						for (i = 0; i < 8; i++)
						{
							context_data_val = (context_data_val << 1) | (value & 1);
							if (context_data_position == bitsPerChar - 1)
							{
								context_data_position = 0;
								context_data.append(getCharFromInt(context_data_val));
								context_data_val = 0;
							}
							else
							{
								context_data_position++;
							}
							value = value >> 1;
						}
					}
					else
					{
						value = 1;
						for (i = 0; i < context_numBits; i++)
						{
							context_data_val = (context_data_val << 1) | value;
							if (context_data_position == bitsPerChar - 1)
							{
								context_data_position = 0;
								context_data.append(getCharFromInt(context_data_val));
								context_data_val = 0;
							}
							else
							{
								context_data_position++;
							}
							value = 0;
						}
						value = int(context_w[0]);
						for (i = 0; i < 16; i++)
						{
							context_data_val = (context_data_val << 1) | (value & 1);
							if (context_data_position == bitsPerChar - 1)
							{
								context_data_position = 0;
								context_data.append(getCharFromInt(context_data_val));
								context_data_val = 0;
							}
							else
							{
								context_data_position++;
							}
							value = value >> 1;
						}
					}
					context_enlargeIn--;
					if (context_enlargeIn == 0)
					{
						context_enlargeIn = (int)pow(2, context_numBits);
						context_numBits++;
					}
					context_mapToCreate.erase(context_w);
				}
				else
				{
					value = context_map[context_w];
					for (i = 0; i < context_numBits; i++)
					{
						context_data_val = (context_data_val << 1) | (value & 1);
						if (context_data_position == bitsPerChar - 1)
						{
							context_data_position = 0;
							context_data.append(getCharFromInt(context_data_val));
							context_data_val = 0;
						}
						else
						{
							context_data_position++;
						}
						value = value >> 1;
					}
				}
				context_enlargeIn--;
				if (context_enlargeIn == 0)
				{
					context_enlargeIn = (int)pow(2, context_numBits);
					context_numBits++;
				}
				//Add wc to the map
				context_map[context_wc] = context_dictSize++;
				context_w = context_c;
			}
		}
		//Output the code for w
		if (context_w.empty()==false)
		{
			if (context_mapToCreate.find(context_w)!= context_mapToCreate.end())
			{
				if (int(context_w[0]) < 256)
				{
					for (i = 0; i < context_numBits; i++)
					{
						context_data_val = (context_data_val << 1);
						if (context_data_position == bitsPerChar - 1)
						{
							context_data_position = 0;
							context_data.append(getCharFromInt(context_data_val));
							context_data_val = 0;
						}
						else
						{
							context_data_position++;
						}
					}
					value = int(context_w[0]);
					for (i = 0; i < 8; i++)
					{
						context_data_val = (context_data_val << 1) | (value & 1);
						if (context_data_position == bitsPerChar - 1)
						{
							context_data_position = 0;
							context_data.append(getCharFromInt(context_data_val));
							context_data_val = 0;
						}
						else
						{
							context_data_position++;
						}
						value = value >> 1;
					}
				}
				else
				{
					value = 1;
					for (i = 0; i < context_numBits; i++)
					{
						context_data_val = (context_data_val << 1) | value;
						if (context_data_position == bitsPerChar - 1)
						{
							context_data_position = 0;
							context_data.append(getCharFromInt(context_data_val));
							context_data_val = 0;
						}
						else
						{
							context_data_position++;
						}
						value = 0;
					}
					value = int(context_w[0]);
					for (i = 0; i < 16; i++)
					{
						context_data_val = (context_data_val << 1) | (value & 1);
						if (context_data_position == bitsPerChar - 1)
						{
							context_data_position = 0;
							context_data.append(getCharFromInt(context_data_val));
							context_data_val = 0;
						}
						else
						{
							context_data_position++;
						}
						value = value >> 1;
					}
				}
				context_enlargeIn--;
				if (context_enlargeIn == 0)
				{
					context_enlargeIn = (int)pow(2, context_numBits);
					context_numBits++;
				}
				context_mapToCreate.erase(context_w);
			}
			else
			{
				value = context_map[context_w];
				for (i = 0; i < context_numBits; i++)
				{
					context_data_val = (context_data_val << 1) | (value & 1);
					if (context_data_position == bitsPerChar - 1)
					{
						context_data_position = 0;
						context_data.append(getCharFromInt(context_data_val));
						context_data_val = 0;
					}
					else
					{
						context_data_position++;
					}
					value = value >> 1;
				}
			}
			context_enlargeIn--;
			if (context_enlargeIn == 0)
			{
				context_enlargeIn = (int)pow(2, context_numBits);
				context_numBits++;
			}
		}
		//Mark the end of the stream
		value = 2;
		for (i = 0; i < context_numBits; i++)
		{
			context_data_val = (context_data_val << 1) | (value & 1);
			if (context_data_position == bitsPerChar - 1)
			{
				context_data_position = 0;
				context_data.append(getCharFromInt(context_data_val));
				context_data_val = 0;
			}
			else
			{
				context_data_position++;
			}
			value = value >> 1;
		}

		//Flush the last char
		while (true)
		{
			context_data_val = (context_data_val << 1);
			if (context_data_position == bitsPerChar - 1)
			{
				context_data.append(getCharFromInt(context_data_val));
				break;
			}
			else context_data_position++;
		}
		return context_data;
	}
	
	/*static CString decompress(CString compressed)
	{
		if (compressed == null) return "";
		if (compressed == "") return null;
		return _decompress(compressed.length(), 32768, compressed);
	}*/

	
	
	static wstring _decompress(int length, int resetValue, GetNextValue getNextValue, wstring &compressed)
	{
		map<int, wstring> maps;
		int next, enlargeIn = 4, dictSize = 4, numBits = 3, i, bits, resb, maxpower, power;
		int c = 0;
		wstring entry, w;

		wstring result;
		auto data = dataStruct();
		data.val = getNextValue(compressed,0);
		data.position = resetValue;
		data.index = 1;

		for (i = 0; i < 3; i++)
		{
			maps[i] = i;
		}
		
		bits = 0;
		maxpower = (int)pow(2, 2);
		power = 1;
		while (power != maxpower)
		{
			resb = data.val & data.position;
			data.position >>= 1;
			if (data.position == 0)
			{
				data.position = resetValue;
				data.val = getNextValue(compressed,data.index++);
			}
			bits |= (resb > 0 ? 1 : 0) * power;
			power <<= 1;
		}
		
		switch (next = bits)
		{
		case 0:
			bits = 0;
			maxpower = (int)pow(2, 8);
			power = 1;
			while (power != maxpower)
			{
				resb = data.val & data.position;
				data.position >>= 1;
				if (data.position == 0)
				{
					data.position = resetValue;
					data.val = getNextValue(compressed,data.index++);
				}
				bits |= (resb > 0 ? 1 : 0) * power;
				power <<= 1;
			}
			c = bits;
			break;
		case 1:
			bits = 0;
			maxpower = (int)pow(2, 16);
			power = 1;
			while (power != maxpower)
			{
				resb = data.val & data.position;
				data.position >>= 1;
				if (data.position == 0)
				{
					data.position = resetValue;
					data.val = getNextValue(compressed,data.index++);
				}
				bits |= (resb > 0 ? 1 : 0) * power;
				power <<= 1;
			}
			c = bits;
			break;
		case 2:
			return wstring();
		}
		maps[3] = wchar_t(c);
		w = wchar_t(c);
		result+= wchar_t(c);
		//return CString();
		while (true)
		{
			if (data.index > length)
			{
				return wstring();
			}

			bits = 0;
			maxpower = (int)pow(2, numBits);
			power = 1;
			while (power != maxpower)
			{
				resb = data.val & data.position;
				data.position >>= 1;
				if (data.position == 0)
				{
					data.position = resetValue;
					data.val = getNextValue(compressed,data.index++);
				}
				bits |= (resb > 0 ? 1 : 0) * power;
				power <<= 1;
			}

			switch (c = bits)
			{
			case 0:
				bits = 0;
				maxpower = (int)pow(2, 8);
				power = 1;
				while (power != maxpower)
				{
					resb = data.val & data.position;
					data.position >>= 1;
					if (data.position == 0)
					{
						data.position = resetValue;
						data.val = getNextValue(compressed,data.index++);
					}
					bits |= (resb > 0 ? 1 : 0) * power;
					power <<= 1;
				}

				maps[dictSize++] = f(bits);
				c = dictSize - 1;
				enlargeIn--;
				break;
			case 1:
				bits = 0;
				maxpower = (int)pow(2, 16);
				power = 1;
				while (power != maxpower)
				{
					resb = data.val & data.position;
					data.position >>= 1;
					if (data.position == 0)
					{
						data.position = resetValue;
						data.val = getNextValue(compressed,data.index++);
					}
					bits |= (resb > 0 ? 1 : 0) * power;
					power <<= 1;
				}
				maps[dictSize++] = f(bits);
				c = dictSize - 1;
				enlargeIn--;
				break;
			case 2:
				return result;
			}

			if (enlargeIn == 0)
			{
				enlargeIn = (int)pow(2, numBits);
				numBits++;
			}

			if (maps.find(c)!= maps.end())
			{
				entry = maps[c];
			}
			else
			{
				if (c == dictSize)
				{
					entry = w + w[0];
				}
				else
				{
					return wstring();
				}
			}
			result+=entry;

			//Add w+entry[0] to the map.
			maps[dictSize++] = w + entry[0];
			enlargeIn--;
			w = entry;
			if (enlargeIn == 0)
			{
				enlargeIn = (int)pow(2, numBits);
				numBits++;
			}
		}
	}
};
