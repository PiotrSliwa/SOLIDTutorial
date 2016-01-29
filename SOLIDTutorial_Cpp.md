# SOLIDTutorial (C++)
This version of the tutorial contains examples in C++ language.

## Introduction
In this tutorial you will be put in the middle of an imaginary world where you are a programmer struggling with a typical project. For the sake of the history let's pretend there is the guy who happens to be your best customer. (S)he runs a small accountancy company and always turns to you when needs a new tool. This time (s)he told you that (s)he could really use a calculator that

1. takes an input which is an equation (i.e. `2 + 3`, `4 - 5`, etc)
2. can operate on summation and subtraction
3. prints out the result

As a magnificent, genius programmer after a glimpse at the requirements you estimate the task for an hour and get to work right away.



## 0. No SOLID at all

### Practice
After exactly one hour you send the working application back to your favourite customer. As always, (s)he is incredibly satisfied with your solution and even pay you extra money for the quick deployment. Let's take a look on how it looks without any of the SOLID rules applied:

**SOURCE CODE**

After comipiling and running the code you can easily test how it works:

```
$ ./calculator
TODO
```

You think in your mind "Okay, it's fine, I'm sure I'll never have to work on it again anyway" and forget about this piece of art. Days pass by and you continue to work on countless other projects. After some time your favourite client contacts with you and says that (s)he will need your help again, because the calculator does not fulfill the new requirements that emerged during the days. Fortunately, the task isn't going to be difficult.
(S)he claims that althought the program is extremely useful, her/his workers sometimes make errors in equations - they use to write `6 = 2` instead of `6 + 2`, `9 p 2` instead of `9 - 2`, etc. and it's quite challenging for them to understand why the calculator tells them about some errors. That's why (s)he asks you to modify the error messages so that they inform users about the details of committed mistake which may work as follows:

```
$ ./calculator
TODO
```

You open calculator's source code and begin to analyze it. Wow, as it turns out it's a tough nut to crack. You must trace the whole flow in one, big file. So you go to the top and then, line by line, try to understand how it works in order to determine the place that needs modification. Of course, you are lucky that it is your own code you are analyzing and it's actually not so big anyway but imagine how hard would it be with some 1000-line-long file written by someone not using you style of coding. It could easily take a few hours or even days to cover all places, not even to mention how it would impact your sanity to operate on so much - in fact - useless information.



## 1. S: Single Responsibility Principle (SRP)

### Theory
The above issue is actually very popular. As programmers we usually deal with short deadlines and exuberant specification which results in code that is developed in a hurry. That means many of us would just put his/her code snippet into a place just to make it work and move to a next task. Inevitably, there have been numerous situations where someone committed something like this:

``` C++
void send(const Message& message, Port& port)
{
    if (message.type == NOT_SUPPORTED) return;  // this line has been added because specification had been changed and now
                                                // a feature is not supposed to send a message when `type` field is "NOT_SUPPORTED"
    port.add(message);
    port.flush();
}
```

The implemented solution is quick and surely simple. However, it makes the function do more than only what its name suggests - now, not only it sends a message, but also validates whether the message shall be sent at all. Therefore, the name is incorrect and worse - the function itself has become highly error-prone. When someone uses it to send a message, (s)he can never be sure that it will be sent whatsoever. Sometimes a message may be supposed to be sent even if `type` field is `NOT_SUPPORTED`. What if someone whould want to send the not upoorted message to a special, logger port:

``` C++
void send(const Message& message, Port& port)
{
    if (message.type == NOT_SUPPORTED) return; 
    port.add(message);
    port.flush();
}

// (...)

void dispatch(const Message& message)
{
    if (message.type == NOT_SUPPORTED)
    {
        send(message, loggerPort);  // `loggerPort` will never receive the message because of the SRP rule violation in `send` function
    }
    else
    {
        send(message, handlerPort);
    }
}
```

Obviously, usage errors could be avoided by changing the function's name from `send` to `sendWhenTypeSupported`. Nevertheless, when name contains a codition, it leads to another problem. Let's try to debug such a short code:

``` C++
void send(const Message& message, Port& port)
{
    if (message.type == NOT_SUPPORTED) return; 
    port.add(message);
    port.flush();
}

// (...)

bool tryToSendSupportedMessage(const Message& message)
{
    sendWhenTypeSupported(message, globalPort);  // 1. Send message
    while (globalPort.getStatus() == BUSY);      // 2. Wait for the port to forward all messages - let's assume getStatus returns
                                                 //    `EMPTY` when there is no message waiting to be forwarded from the port
                                                 //    `BUSY` when there is at least one message being forwarded
                                                 //    `ERROR` when the port broke down
    if (globalPort.getStatus() == EMPTY)         // 3. When status is `EMPTY` (not `ERROR`) return true (notify about success)
    {
        return true;
    }
    return false;
}
```

Can you see the problem? Of course! When there was a message with type set to `NOT_SUPPORTED`, `sendWhenTypeSupported()` would never add the message to the port and therefore the status would always be `EMPTY` -> `tryToSendSupportedMessage` would return `true` even if the message wasn't sent at all! That's a nasty bug.

Now, look at the improved example:

``` C++
void send(const Message& message, Port& port)
{
    port.add(message);
    port.flush();
}

void isMessageSupported(const Message& message)
{
    return message.type != NOT_SUPPORTED;
}

// (...)

void waitForPort(const Port& port)
{
    while (port.getStatus() == BUSY);
}

void isPortEmpty(const Port& port)
{
    return globalPort.getStatus() == EMPTY;
}

bool tryToSendSupportedMessage(const Message& message)
{
    if (isMessageSupported(message))
    {
        send(message, globalPort);
        waitForPort(globalPort);
        return isPortEmpty(globalPort);
    }
    return false;
}
```

Now, it should work properly. Isn't it clearer to read, too? What we did is that we split the one, big, many-in-one function into multiple small ones. Thanks to that, we could defer the development of logic until all the complicated, low-level implementational details are nicely packed into readable functions. Now, when you read `tryToSendSupportedMessage()` function, you can almost read it like a high-level documentation:

``` C++
if (isMessageSupported(message))        // if `message` is supported
{                                       // {
    send(message, globalPort);          //      send `message` to `globalPort`
    waitForPort(globalPort);            //      and wait for `globalPort` to finish business
    return isPortEmpty(globalPort);     //      then return whether `globalPort` is empty
}                                       // }
return false;                           // otherwise return false
```

Another problem introduced by the bigger `sendWhenTypeSupported` function is - as you probably guess - that it's less reusable than `send`. It can be used only if developer restricts that only messages with `type` field other than `NOT_SUPPORTED` are to be sent. Just imagine a monster like `sendSupportedMessageToRecentlyCreatedUser`. Even though it seems just evil, such things happen in real world code (much too often). Note, that there are many things that could force a need to modify the function - at least change of message support condition, user storage implementation and message sending implementation. Sum it up with the fact, that there are plentiful of highly specialized functions like

``` C++
void sendSupportedMessageToRecentlyCreatedUser(...) { ... }
void sendUnsupportedMessageToLogger(...) { ... }
void sendMessageToGlobalPort(...) { ... }
// (...)
```

and you end up having nightmares each time there's a demand for a slight change with message sending mechanism, because to do that you have to change and test an enormous amount of code to achieve very little. It means that **the cost of code maintenence begins to grow exponentially**.

Remember: **it's easier to build a house of many small bricks than to carve the same in one, big rock**. That means, it's better to create many small functions and use them freely than to create big ones that cannot be reused and needs modification anytime something changes in code.

The same rule applies to objects (which will be brought up in *Practice* paragraph.


### Practice


## 2. Open-Closed Principle (OCP)

### Theory

### Practice

## 3. Liskov Substitution Principle (LSP)

### Theory

### Practice

## 4. Interface Segregation Principle (ISP)

### Theory

### Practice

## 5. Dependency Inversion Principle (DIP)

### Theory

### Practice
