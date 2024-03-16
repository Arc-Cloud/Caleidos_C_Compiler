int unary(int x, int y) {
    x = -x;
    x = ~x;
    x = !x;
    y = ++x;
    return y;
}
