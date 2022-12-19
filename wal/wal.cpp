#include <cstdio>
#include <cinttypes>
#include <cmath>
#include <vector>

using namespace std;

struct BigNumber;
extern void Add(BigNumber& a, BigNumber& b, BigNumber& c);
extern void Substract(BigNumber& a, BigNumber& b, BigNumber& c);
extern void Multiply(BigNumber& a, BigNumber& b, BigNumber& c);
extern void Divide(BigNumber& a, BigNumber& b, BigNumber& c);
extern void Modulo(BigNumber& a, BigNumber& b, BigNumber& c);
extern signed char Compare(BigNumber& a, BigNumber& b);
extern void GCD(BigNumber& a, BigNumber& b, BigNumber& c);
extern void LCM(BigNumber& a, BigNumber& b, BigNumber& c);

struct BigNumber
{
    vector<char> digits;
    signed char sign;

public:
    void Initialize()
    {
        digits.clear();
        digits.push_back(0);
        sign = 1;
    }

    void Initialize(int64_t v)
    {
        digits.clear();

        if (v == 0LL)
        {
            Initialize();
            return;
        }

        sign = (v < 0) ? -1 : 1;
        if (v < 0)
        {
            v = -v;
        }
 
        while(v > 0)
        {
            digits.push_back(v % 10);
            v /= 10;
        }
    }

    void Initialize(BigNumber& v)
    {
        digits.clear();

        sign = v.sign;

        for(int i = 0; i <= v.LastDigit(); i++)
        {
            digits.push_back(v.digits[i]);
        }
    }

    int LastDigit()
    {
        return digits.size() - 1;
    }

    void SetLastDigit(int v)
    {
        while (LastDigit() < v)
        {
            digits.push_back(0);
        }
    }

    void Print()
    {
        if(sign == -1)
        {
            printf("-");
        }
        for(int i = LastDigit(); i >= 0; i--)
        {
            printf("%c", '0' + digits[i]);
        }
    }

    void ZeroJustify()
    {
        while(LastDigit() > 0 && digits[LastDigit()] == 0)
        {
            digits.pop_back();
        }

        if (LastDigit() == 0 && digits[0] == 0)
        {
            sign = 1;
        }
    }

    void DigitShift(int d)
    {
        if(LastDigit() == 0 && digits[0] == 0) return;

        SetLastDigit(LastDigit() + d);

        for(int i = LastDigit() - d; i>= 0; i--)
        {
            digits[i+d] = digits[i];
        }
        for (int i = 0; i < d; i++)
        {
            digits[i] = 0;
        }
    }
};

void Add(BigNumber& a, BigNumber& b, BigNumber& c)
{
    c.Initialize();

    if(a.sign == b.sign)
    {
        c.sign = a.sign;
    }
    else
    {
        if (a.sign < 0)
        {
            a.sign = 1;
            Substract(b, a, c);
            a.sign = -1;
        }
        else
        {
            b.sign = 1;
            Substract(a, b, c);
            b.sign = -1;
        }
        return;
    }

    int length = max(a.LastDigit(), b.LastDigit()) + 1;
    a.SetLastDigit(length);
    b.SetLastDigit(length);
    c.SetLastDigit(length);

    signed char carry = 0;
    for(int i = 0; i <= c.LastDigit(); i++)
    {
        c.digits[i] = (a.digits[i] + b.digits[i] + carry) % 10;
        carry = (a.digits[i] + b.digits[i] + carry) / 10;
    }

    a.ZeroJustify();
    b.ZeroJustify();
    c.ZeroJustify();
}

void Substract(BigNumber& a, BigNumber& b, BigNumber& c)
{
    c.Initialize();

    if (a.sign < 0 || b.sign < 0)
    {
        b.sign *= -1;
        Add(a, b, c);
        b.sign *= -1;
        return;
    }

    if(Compare(a,b) > 0)
    {
        Substract(b,a,c);
        c.sign = -1;
        return;
    }

    int length = max(a.LastDigit(), b.LastDigit());
    a.SetLastDigit(length);
    b.SetLastDigit(length);
    c.SetLastDigit(length);
    
    signed char borrow = 0;
    for (int i = 0; i <= c.LastDigit(); i++)
    {
        signed char v = a.digits[i] - b.digits[i] - borrow;
        if (v >= 0)
        {
            borrow = 0;
        }
        else
        {
            v += 10;
            borrow = 1;
        }
        c.digits[i] = v % 10;
    }

    a.ZeroJustify();
    b.ZeroJustify();
    c.ZeroJustify();
}

void Multiply(BigNumber& a, BigNumber& b, BigNumber& c)
{
    BigNumber row;
    BigNumber tmp;

    c.Initialize();
    row.Initialize(a);

    for(int i = 0; i <= b.LastDigit(); i++)
    {
        for(int j = 1; j <= b.digits[i]; j++)
        {
            Add(c, row, tmp);
            c.Initialize(tmp);
        }
        row.DigitShift(1);
    }

    c.sign = a.sign * b.sign;
    c.ZeroJustify();
}

void Divide(BigNumber& a, BigNumber& b, BigNumber& c)
{
    BigNumber row;
    BigNumber tmp;
    signed char asign, bsign;

    c.Initialize();
    row.Initialize();
    tmp.Initialize();

    c.sign = a.sign * b.sign;
    asign = a.sign;
    bsign = b.sign;
    a.sign = b.sign = 1;
    
    c.SetLastDigit(a.LastDigit());

    for (int i = a.LastDigit(); i>=0; i--)
    {
        row.DigitShift(1);
        row.digits[0] = a.digits[i];
        c.digits[i] = 0;

        while(Compare(row, b) <= 0)
        {
            c.digits[i]++;
            Substract(row, b, tmp);
            row.Initialize(tmp);
        }
    }

    c.ZeroJustify();

    a.sign = asign;
    b.sign = bsign;
}

void Modulo(BigNumber& a, BigNumber& b, BigNumber& c)
{
    BigNumber row;
    BigNumber tmp;
    signed char asign, bsign;

    c.Initialize();
    tmp.Initialize();

    asign = a.sign;
    bsign = b.sign;
    a.sign = b.sign = 1;

    for (int i = a.LastDigit(); i>=0; i--)
    {
        c.DigitShift(1);
        c.digits[0] = a.digits[i];

        while(Compare(c, b) <= 0)
        {
            Substract(c, b, tmp);
            c.Initialize(tmp);
        }
    }

    c.ZeroJustify();

    a.sign = asign;
    b.sign = bsign;
    c.sign = a.sign * b.sign;
}

signed char Compare(BigNumber& a, BigNumber& b)
{
    if (a.sign < 0 && b.sign > 0) return 1;
    if (a.sign > 0 && b.sign < 0) return -1;

    if (b.LastDigit() > a.LastDigit()) return a.sign;
    if (a.LastDigit() > b.LastDigit()) return -a.sign;

    for (int i = a.LastDigit(); i >= 0; i--)
    {
        if(a.digits[i] > b.digits[i]) return -a.sign;
        if(b.digits[i] > a.digits[i]) return a.sign;
    }

    return 0;
}

uint64_t gcd(uint64_t p, uint64_t q)
{
    while (q != 0)
    {
        uint64_t r = p%q;
        p = q;
        q = r;
    }

    return p;
}

void GCD(BigNumber& a, BigNumber& b, BigNumber& c)
{
    BigNumber q;
    BigNumber r;

    c.Initialize(a);
    q.Initialize(b);

    while (!(q.LastDigit() == 0 && q.digits[0] == 0))
    {
        Modulo(c, q, r);
        c.Initialize(q);
        q.Initialize(r);
    }
}

uint64_t lcm(uint64_t p, uint64_t q)
{
    return (p / gcd(p,q)) * q;
}

void LCM(BigNumber& a, BigNumber& b, BigNumber& c)
{
    BigNumber tmp;

    GCD(a, b, c);
    Divide(a, c, tmp);
    Multiply(tmp, b, c);
}

int main()
{
    int n, r, l;
    vector<vector<int>> inputs;
    vector<int> outputs;
    vector<BigNumber> cycle;
    vector<BigNumber> packet;
    BigNumber max;

    scanf("%d", &n);
    inputs.resize(n+1);
    outputs.resize(n+1);

    for(int i = 1; i <= n; i++)
    {
        scanf("%d", &r);
        outputs[i] = r;

        for(int j = 1; j <= r; j++)
        {
            scanf("%d", &l);
            inputs[l].push_back(i);
        }
    }

    cycle.resize(n+1);
    packet.resize(n+1);

    cycle[1].Initialize(outputs[1]);
    packet[1].Initialize(1);
    if (outputs[1] != 0)
    {
        max.Initialize(outputs[1]);
    }
    else
    {
        max.Initialize(1);
    }

    BigNumber c;
    BigNumber o;
    BigNumber m;
    BigNumber a;
    BigNumber cin;
    BigNumber pin;

    for(int i = 2; i <= n; i++)
    {
        cycle[i].Initialize(1);
        packet[i].Initialize(0);

        for(int in : inputs[i])
        {
            cin.Initialize(cycle[in]);
            pin.Initialize(packet[in]);

            LCM(cycle[i], cin, c);

            if(Compare(cycle[i], c) != 0)
            {
                Divide(c, cycle[i], m);
                Multiply(packet[i], m, a);
                cycle[i].Initialize(c);
                packet[i].Initialize(a);
            }
            if(Compare(cin, c) != 0)
            {
                Divide(c, cin, m);
                Multiply(pin, m, a);
                pin.Initialize(a);
            }

            Add(packet[i], pin, a);
            packet[i].Initialize(a);
        }

        if (outputs[i] > 0)
        {
            o.Initialize(outputs[i]);
            LCM(packet[i], o, c);

            if(Compare(packet[i], c) != 0)
            {
                Divide(c, packet[i], m);
                Multiply(cycle[i], m, a);
                packet[i].Initialize(c);
                cycle[i].Initialize(a);
            }

            Divide(packet[i], o, a);
            packet[i].Initialize(a);
        }

        LCM(max, cycle[i], m);
        max.Initialize(m);
    }

    max.Print();
    printf("\n");

    return 0;
}