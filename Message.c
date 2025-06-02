/* Exercise
IPC using Message Queue

read inputs from the parent process
Enter Name: Keerthana
Enter RegNo: 2021ICT96
Receive Age:23

Message sent successfully

Give the output from the child proccess

Enter Name: Keerthana
Enter RegNo: 2021ICT96
Receive Age:23
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long msg_type;
    char name[50];
    char reg_no[20];
    int age;
};

int main() {
    key_t key;
    int msgid;

    // Generate unique key
    key = ftok("msg_queue", 65);

    // Create message queue and validate
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Message queue creation failed");
        exit(1);
    }

    struct message msg;
    msg.msg_type = 1;

    printf("Enter Name: ");
    fgets(msg.name, sizeof(msg.name), stdin);
    msg.name[strcspn(msg.name, "\n")] = 0; // Remove newline

    printf("Enter RegNo: ");
    fgets(msg.reg_no, sizeof(msg.reg_no), stdin);
    msg.reg_no[strcspn(msg.reg_no, "\n")] = 0; // Remove newline

    printf("Enter Age: ");
    scanf("%d", &msg.age);
    getchar(); // Clear newline left by scanf

    // Send message
    if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(msg.msg_type), 0) == -1) {
        perror("Message send failed");
        exit(1);
    }

    printf("Message sent successfully.\n");
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long msg_type;
    char name[50];
    char reg_no[20];
    int age;
};

int main() {
    key_t key;
    int msgid;

    // Generate unique key
    key = ftok("msg_queue", 65);

    // Retrieve message queue and validate
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Message queue retrieval failed");
        exit(1);
    }

    struct message msg;

    // Receive message
    if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(msg.msg_type), 1, 0) == -1) {
        perror("Message receive failed");
        exit(1);
    }

    // Display received data
    printf("Received Name: %s\n", msg.name);
    printf("Received RegNo: %s\n", msg.reg_no);
    printf("Received Age: %d\n", msg.age);

    // Remove message queue after use
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
