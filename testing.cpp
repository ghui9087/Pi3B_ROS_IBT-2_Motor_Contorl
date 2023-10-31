#include <stdio.h>

int main() {
    int data[4] = {0, 0, 0, 0};
    char buffer[50];

    // Add debug print statements to check the values of the data array
    printf("data[0]: %d\n", data[0]);
    printf("data[1]: %d\n", data[1]);
    printf("data[2]: %d\n", data[2]);
    printf("data[3]: %d\n", data[3]);

    sprintf(buffer, "Current Speed %c, %d%%, %c, %d%%", (char)data[2], data[0], (char)data[3], data[1]);

    printf("Buffer: %s\n", buffer); // Print the buffer to check its content

    // Assuming the setup and connection of Serial and LED pins are correct
    // Serial.println(buffer);
    // digitalWrite(ledPin, HIGH);
    // delay(50);
    // digitalWrite(LED_BUILTIN, LOW);

    return 0;
}
