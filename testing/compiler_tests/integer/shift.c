int shift_operations(int x, int shiftLeft, int shiftRight) {
    int leftShifted = x << shiftLeft;
    int rightShifted = x >> shiftRight;

    return leftShifted + rightShifted;
}
