int op_assign(int x, int y) {
    x += y + 20;
    x -= y + 5;
    x *= 50 / x;
    x /= y - 9*2;
    x %= 18;
    x &= 13;
    x |= 10;
    x ^= 5;
    x >>= 3;
    x <<= 3;
    return x;
}
