float f();

int main()
{
    return !(fabs(f() - 3.5) < 0.0001);
}
