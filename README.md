# ChaTTY

ChaTTY is a decentralized CLI chat client originally made to work across the 42 Madrid campus network, although it could also work in other workplace or local network environments.

It is still a **work in progress**, but the basic chat functionality is already working.

The idea is pretty simple: at 42, everyone is usually sitting at their computers with a terminal open. ChaTTY is meant to let you find and talk to other people directly from the terminal, without relying on a central server.

## How it works

ChaTTY currently has three threads running in parallel.

### 1. The Beacon

The Beacon sends a small message every 3 seconds to users within the same IP ranges.

The message looks like:

```text
1;johndoe;c2r6s11;
```

The format is:

```text
<status>;<username>;<location>;
```

Where:

* `1` means the user is online.
* `0` means the user is offline.
* `username` is taken from the user's environment.
* `location` represents where the user is on campus.

The message is intentionally kept simple so it can be parsed quickly, especially when there may be 500+ users on the network.

Losing one of these messages isn't really a problem. They are mainly used to discover new users, and the next beacon will be sent a few seconds later anyway.

### 2. The Receiver

The second thread handles incoming UDP messages from other users.

When a message is received, it is parsed and the username is checked against my own simple hash table implementation to see if the user already exists.

If the user is new, we store:

* Their username
* Their location
* Their `sockaddr` information
* Their TCP port

The user is then added to the shared users table and a message is displayed showing that they have come online.

Originally, I also sent colour information through the UDP message, although I am currently considering removing this.

If the user is already in our contacts table, nothing needs to be done.

Since multiple threads can access the users table, it is protected with mutexes to avoid race conditions when reading or writing to it.

### 3. TCP Receiver

The third thread is responsible for receiving actual chat messages through TCP.

The message format is similar to the UDP messages, but TCP is used for the actual communication so that we can have an acknowledgement routine and make sure the message was received.

Messages are displayed in a format similar to:

```text
<johndoe::c2r3s11><You> Heyyy hows it going?
```

To send a message, you can simply run:

```bash
chatty <username> <message>
```

ChaTTY will open a TCP connection to that user and send the message.

There are also commands for changing things like your colour signature.

## Current state

ChaTTY is still a work in progress.

The current version works as a basic decentralized chat system, but there are several things I want to change and improve.

I am currently **porting the project from C to C++**, so some functionality may be incomplete, unstable, or temporarily not work while the rewrite is in progress.

Because of this, the current version should be considered a work in progress rather than a stable release.

## Future plans

### Encryption

Currently, messages sent through TCP are sent in plain text.

One of the main things I want to add is some form of shared-key encryption so that messages are not transmitted unencrypted.

### Daemon

The bigger goal for ChaTTY is to turn it into a daemon that runs in the background.

Ideally, it would be able to pipe its input and output directly to the user's terminal, meaning you wouldn't need to keep a separate terminal open just for ChaTTY.

### Group chats

I also want to add support for group chats so multiple users can communicate in the same conversation.

### 1-to-1 TTY conversations

Another idea is being able to start a 1-to-1 conversation directly through a specific TTY instance.

This would fit more closely with the original idea of ChaTTY: having chat available directly where you are already working, rather than requiring a separate application window.

## The idea

The current implementation is mostly an experiment in networking, concurrency, and systems programming.

The long-term vision is to turn it from a simple CLI chat application into a small background service that can provide **encrypted, decentralized communication directly through the terminal**, without relying on a central server.
