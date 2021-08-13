#include <iostream>
#include <string>
#include <fstream>
#include <memory>

/*
    ITransmitter  Diagnoser   Diagnoser
        \\       //          //
        Transmitter   Receiver
                 \\  //
                  Radio
 */

/* virtual:

    ITransmitter  Diagnoser
        \\       /     \
        Transmitter   Receiver
                 \\  //
                  Radio   <-- call Diagnoser constructor
 */

struct Diagnoser {
    Diagnoser(const std::string &file_name) 
      : file_name(file_name) {
    }

    virtual void diagnose(const std::string &message) {
        const auto line = message + '\n';
        std::ofstream out(file_name.c_str(), std::ofstream::app);
        out.write(line.c_str(), line.length());
        out.close();
    }

    virtual ~Diagnoser() = default;

private:
    const std::string file_name;
};

struct ITransmitter {
    virtual void send(const std::string &text) = 0;
    virtual ~ITransmitter() = default;
};

struct Transmitter : ITransmitter, virtual protected Diagnoser {
    Transmitter() : Diagnoser("transmitter.txt") {
        std::cout << "[Transmitter] creating...\n";
    }

    void send(const std::string &text) override {
        std::cout << "[Transmitter] sending: " << text << '\n';
        diagnose("sent: " + text);
    }

    ~Transmitter() { /* early binding vs late binding */
        std::cout << "[Transmitter] destroying...\n";
    }
};

struct Receiver : virtual protected Diagnoser {
    Receiver() : Diagnoser("receiver.txt") {
    }

    std::string receive() {
        std::string text = "Received text.";
        diagnose("received: " + text);
        return text;
    }
};

struct Radio : Transmitter, Receiver {
    Radio() : Diagnoser("radio.txt") {
    }

    void send(const std::string &text) override {
        std::cout << "[Radio] sending...\n";
        Transmitter::diagnose("radio is sending: " + text);
        constexpr int num_of_chars_to_send = 10;
        for (std::string::size_type i = 0; i < text.length(); i += num_of_chars_to_send) {
            if (i + num_of_chars_to_send > text.length()) {
                Transmitter::send(text.substr(i, text.length() - i));
            }
            else {
                Transmitter::send(text.substr(i, num_of_chars_to_send));
            }
        }
    }
};

int main() {
    //std::unique_ptr<Transmitter> transmitter = std::make_unique<Radio>();
    //transmitter->send("Hello, hello, hello, hello.");

    Transmitter t;
    t.send("transmitter");

    Radio radioA;           // stack
    radioA.send("radio A");

    std::unique_ptr<Radio> radio = std::make_unique<Radio>();  // heap
    radio->send("Hello, hello, hello, hello.");
    radio->receive();

    return 0;
}

