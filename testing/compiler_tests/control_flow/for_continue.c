int f() {
    int i;
    int x = 0;
    for (i = 1; i <= 10; i++) {
        if (i % 2 != 0) {
            continue;
        }
        x++;
    }
    return x;
}
