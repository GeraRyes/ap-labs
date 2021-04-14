extern int infof(const char *format, ...);
extern int warnf(const char *format, ...);
extern int errorf(const char *format, ...);
extern int panicf(const char *format, ...);

int main() {
    infof("Info example: number %d\n", 1);
    warnf("Warning example number %d\n", 2);
    errorf("Error example: number %d\n", 3);
    panicf("Panic example: number %d\n", 4);
    return 0;
}