#include "../../third_party/yojimbo/include/yojimbo.h"

#include <iostream>

inline int GetNumBitsForMessage(uint16_t sequence)
{
    static int messageBitsArray[] = { 1, 320, 120, 4, 256, 45, 11, 13, 101, 100, 84, 95, 203, 2, 3, 8, 512, 5, 3, 7, 50 };
    const int modulus = sizeof(messageBitsArray) / sizeof(int);
    const int index = sequence % modulus;
    return messageBitsArray[index];
}

class GameMessage : public yojimbo::Message
{
public:
    GameMessage(uint16_t sequence = 0) : sequence(sequence) {}
    uint16_t sequence;

    template <typename Stream> bool Serialize(Stream& stream)
    {
        serialize_bits(stream, sequence, 16);

        int numBits = GetNumBitsForMessage(sequence);
        int numWords = numBits / 32;
        uint32_t dummy = 0;
        for (int i = 0; i < numWords; ++i)
            serialize_bits(stream, dummy, 32);
        int numRemainderBits = numBits - numWords * 32;
        if (numRemainderBits > 0)
            serialize_bits(stream, dummy, numRemainderBits);

        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};

class GameBlockMessage : public yojimbo::BlockMessage
{
public:
    GameBlockMessage(uint16_t sequence = 0) : sequence(sequence) {}
    uint16_t sequence;

    template <typename Stream> bool Serialize(Stream& stream)
    {
        serialize_bits(stream, sequence, 16);
        return true;
    }

    YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};

enum GameMessageType
{
    GAME_MESSAGE,
    GAME_BLOCK_MESSAGE,
    NUM_GAME_MESSAGE_TYPES
};

class GameMessageFactory : public yojimbo::MessageFactory
{
public:
    GameMessageFactory(yojimbo::Allocator& allocator) : MessageFactory(allocator, NUM_GAME_MESSAGE_TYPES) {}
    yojimbo::Message* CreateMessageInternal(int type)
    {
        yojimbo::Message* message;
        yojimbo::Allocator& allocator = GetAllocator();
        (void)allocator;
        switch (type)
        {
        case GAME_MESSAGE:
            message = YOJIMBO_NEW(allocator, GameMessage);
            if (!message)
                return nullptr;
            SetMessageType(message, type);
            return message;
        case GAME_BLOCK_MESSAGE:
            message = YOJIMBO_NEW(allocator, GameBlockMessage);
            if (!message)
                return nullptr;
            SetMessageType(message, type);
            return message;
        }
    }
};

class GameAdapter : public yojimbo::Adapter
{
public:
    GameAdapter() : server(nullptr), factory(nullptr) {}

    void SetServer(yojimbo::Server* server) {
        this->server = server;
    }
    GameMessageFactory* CreateMessageFactory(yojimbo::Allocator& allocator) override
    {
        factory = YOJIMBO_NEW(allocator, GameMessageFactory, allocator);
        return factory;
    }
    void DestroyMessageFactory(yojimbo::Allocator& allocator)
    {
        YOJIMBO_DELETE(allocator, GameMessageFactory, factory);
    }
    void OnServerClientConnected(int clientIndex) override {
        std::cout << "Client connected: " << clientIndex << std::endl;
    }
    void OnServerClientDisconnected(int clientIndex) override {
        std::cout << "Client disconnected: " << clientIndex << std::endl;
    }
    GameMessageFactory* factory;

private:
    yojimbo::Server* server;
};
