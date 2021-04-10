int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main(){
    char* message="'This is the message'";
    infof("Info Message: %s\n",message);
    warnf("Warn Message: %s\n",message);
    errorf("Error Message: %s\n",message);
    panicf("Panic Message: %s\n",message);
}