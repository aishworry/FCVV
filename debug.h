bool setDebug(int baud);
void returnDebug(int n);

bool setDebug(int baud)
{
    Serial.begin(baud);
    returnDebug(0);
    return true;
}
void returnDebug(int n)
{
    switch(n)
    {
        case 0:
        //
        break;
        case 1:
        //
        break;
        default:
        //
        break;
    }
}