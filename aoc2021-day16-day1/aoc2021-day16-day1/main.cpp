//
//  main.c
//  aoc2021-day16-day1
//
//  Created by Nikita Evstigneev on 16.12.2021.
//

#include <stdio.h>
#include <vector>

using namespace std;

class Packet {
public:
    int version;
    int type;
    Packet(int version, int type): version(version), type(type) {}
    static unique_ptr<Packet> read_packets(vector<bool> input, int & bit_idx);
    virtual void print(int tabs) {
        printf("Base packet\n");
    }
    virtual int sum_versions() {
        return version;
    }
    virtual long long execute() {
        return 0;
    }
};

class LitPacket: public Packet {
public:
    long long number;
    LitPacket(int version, int type): Packet(version, type) {}
    void read_number(vector<bool> input, int & bit_idx) {
        bool is_last = false;
        number = 0;
        while (!is_last) {
            is_last = !input[ bit_idx++ ];
            for (int i = 0; i < 4; i += 1) {
                number = number * 2 + input[ bit_idx++ ];
            }
        }
    }
    void print(int tabs) {
        for (int i = 0; i < tabs; i += 1) printf("\t");
        printf("Literal packet (version = %d, type = %d, number = %lld)\n", version, type, number);
    }
    int sum_versions() {
        return version;
    }
    long long execute() {
        return number;
    }
};

class OpPacket: public Packet {
public:
    int len_type;
    int len;
    vector<unique_ptr<Packet>> children;
    OpPacket(int version, int type): Packet(version, type) {}
    void read_body(vector<bool> input, int & bit_idx) {
        len_type = input[ bit_idx++  ];
        len = 0;
        if (len_type == 0) {
            // number of bits
            for (int i = 0; i < 15; i += 1) {
                len = len * 2 + input[ bit_idx++ ];
            }
            // read until we have read "len" bits
            int stop_idx = bit_idx + len;
            while (bit_idx < stop_idx) {
                children.push_back( Packet::read_packets(input, bit_idx) );
            }
        } else {
            // number of subpackets
            len = 0;
            for (int i = 0; i < 11; i += 1) {
                len = len * 2 + input[ bit_idx++ ];
            }
            // read "len" packets
            for (int i = 0; i < len; i += 1) {
                children.push_back( Packet::read_packets(input, bit_idx) );
            }
        }
    }
    void print(int tabs) {
        for (int i = 0; i < tabs; i += 1) printf("\t");
        printf("Operator packet (version = %d, type = %d, len = %d, len_type = %d)\n", version, type, len, len_type);
        for (int i = 0; i < children.size(); i += 1) {
            children[i]->print(tabs + 1);
        }
    }
    int sum_versions() {
        int r = version;
        for (int i = 0; i < children.size(); i += 1) {
            r += children[i]->sum_versions();
        }
        return r;
    }
    virtual long long execute() {
        return 0;
    }
};

class SumPacket: public OpPacket {
public:
    SumPacket(int version, int type): OpPacket(version, type) {}
    long long execute() {
        long long r = 0;
        for (int i = 0; i < children.size(); i += 1) {
            r += children[i]->execute();
        }
        return r;
    }
};

class ProductPacket: public OpPacket {
public:
    ProductPacket(int version, int type): OpPacket(version, type) {}
    long long execute() {
        long long r = 1;
        for (int i = 0; i < children.size(); i += 1) {
            r *= children[i]->execute();
        }
        return r;
    }
};

class MinPacket: public OpPacket {
public:
    MinPacket(int version, int type): OpPacket(version, type) {}
    long long execute() {
        long long r = children[0]->execute(), tmp;
        for (int i = 0; i < children.size(); i += 1) {
            tmp = children[i]->execute();
            if (tmp < r) {
                r = tmp;
            }
        }
        return r;
    }
};

class MaxPacket: public OpPacket {
public:
    MaxPacket(int version, int type): OpPacket(version, type) {}
    long long execute() {
        long long r = children[0]->execute(), tmp;
        for (int i = 0; i < children.size(); i += 1) {
            tmp = children[i]->execute();
            if (tmp > r) {
                r = tmp;
            }
        }
        return r;
    }
};

class GreaterThenPacket: public OpPacket {
public:
    GreaterThenPacket(int version, int type): OpPacket(version, type) {}
    long long execute() {
        return children[0]->execute() > children[1]->execute();
    }
};

class LessThenPacket: public OpPacket {
public:
    LessThenPacket(int version, int type): OpPacket(version, type) {}
    long long execute() {
        return children[0]->execute() < children[1]->execute();
    }
};

class EqualToPacket: public OpPacket {
public:
    EqualToPacket(int version, int type): OpPacket(version, type) {}
    long long execute() {
        return children[0]->execute() == children[1]->execute();
    }
};

unique_ptr<Packet> Packet::read_packets(vector<bool> input, int & bit_idx) {
    int version = 0;
    int type = 0;
    // read version
    for (int i = 0; i < 3; i += 1) {
        version = version * 2 + input[bit_idx];
        bit_idx += 1;
    }
    // read type
    for (int i = 0; i < 3; i += 1) {
        type = type * 2 + input[bit_idx];
        bit_idx += 1;
    }
    if (type == 4) {
        unique_ptr<LitPacket> packet = unique_ptr<LitPacket>(new LitPacket(version, type));
        packet->read_number(input, bit_idx);
        return packet;
    } else {
        unique_ptr<OpPacket> packet;
        switch (type) {
            case 0:
                packet = unique_ptr<SumPacket>(new SumPacket(version, type));
                break;
            case 1:
                packet = unique_ptr<ProductPacket>(new ProductPacket(version, type));
                break;
            case 2:
                packet = unique_ptr<MinPacket>(new MinPacket(version, type));
                break;
            case 3:
                packet = unique_ptr<MaxPacket>(new MaxPacket(version, type));
                break;
            case 5:
                packet = unique_ptr<GreaterThenPacket>(new GreaterThenPacket(version, type));
                break;
            case 6:
                packet = unique_ptr<LessThenPacket>(new LessThenPacket(version, type));
                break;
            case 7:
                packet = unique_ptr<EqualToPacket>(new EqualToPacket(version, type));
                break;
            default:
                printf("WRONG TYPE %d\n", type);
                exit(1);
        }
        packet->read_body(input, bit_idx);
        return packet;
    }
}

vector<bool> hex_to_bin(FILE *f) {
    char hex[2048];
    unsigned short b = 0;
    vector<bool> result;
    fscanf(f, "%s", hex);
    for (int i = 0; i < strlen(hex); i += 1) {
        b = (hex[i] >= 'A') ? (hex[i] - 'A' + 10) : (hex[i] - '0');
        result.push_back(b & 0b1000);
        result.push_back(b & 0b0100);
        result.push_back(b & 0b0010);
        result.push_back(b & 0b0001);
    }
    return result;
}

int main(int argc, const char * argv[]) {
    FILE *f = fopen("input.txt", "r");
    vector<bool> input = hex_to_bin(f);
    fclose(f);
    
    for (int i = 0; i < input.size(); i += 1) {
        printf("%d", input[i] ? 1 : 0);
    }
    printf("\n");
    
    int bit_idx = 0;
    unique_ptr<Packet> result = Packet::read_packets(input, bit_idx);
    result->print(0);
    printf("%lld\n", result->execute());
    
    return 0;
}
