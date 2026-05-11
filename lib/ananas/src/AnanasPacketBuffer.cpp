#include "AnanasPacketBuffer.h"

namespace ananas
{
    PacketBuffer::PacketBuffer(const SystemUtils::LogLevel logLevel)
        : logging(logLevel)
    {
    }

    uint8_t *PacketBuffer::getWritePointer()
    {
        return buffer[writeIndex].rawData();
    }

    void PacketBuffer::write(const AudioPacket &packet)
    {
        const auto prevWriteIndex{writeIndex == 0 ? Constants::PacketBufferCapacity - 1 : writeIndex - 1};
        const auto seqNumDelta{
            packet.header.sequenceNumber == 0 && buffer[prevWriteIndex].header.sequenceNumber == UINT16_MAX
                ? 1
                : packet.header.sequenceNumber - buffer[prevWriteIndex].header.sequenceNumber
        };

        // If the change in sequence number is greater than one or negative ---
        // and it's not unreasonably large, e.g. on receiving a first packet ---
        // this is an out-of-sequence packet scenario. Increment or decrement
        // the write index to accommodate the momentary loss of
        // sequentiality.
        if ((seqNumDelta >= 2 || seqNumDelta <= -1) && abs(seqNumDelta) < 10) {
            if (logging > SystemUtils::LogLevel::Low) {
                Serial.printf("Received out-of-sequence packet: seq %" PRIu16 ", %s%d\n",
                              packet.header.sequenceNumber,
                              seqNumDelta > 0 ? "+" : "",
                              seqNumDelta);
            }

            if (seqNumDelta > 0) {
                for (auto i{1}; i < seqNumDelta; ++i) {
                    incrementWriteIndex();
                }
            } else {
                for (auto i{0}; i >= seqNumDelta; --i) {
                    decrementWriteIndex();
                }
            }

            if (logging > SystemUtils::LogLevel::Low) {
                Serial.printf("Writing at index %d\n", writeIndex);
            }
        }

        buffer[writeIndex] = packet;
        incrementWriteIndex();
    }

    AudioPacket &PacketBuffer::read()
    {
        auto &packet{peek()};
        incrementReadIndex();
        return packet;
    }

    AudioPacket &PacketBuffer::peek()
    {
        return buffer[readIndex];
    }

    size_t PacketBuffer::printTo(Print &p) const
    {
        return p.printf("  Read index: %" PRIu32 ", Write index: %" PRIu32 ", %" PRIu8 " %% full.\n",
                        readIndex,
                        writeIndex,
                        getFillPercent());
    }

    void PacketBuffer::incrementWriteIndex()
    {
        ++writeIndex;
        if (writeIndex >= Constants::PacketBufferCapacity) {
            writeIndex = 0;
        }
    }

    void PacketBuffer::incrementReadIndex()
    {
        // Might be better to avoid a division...
        ++readIndex;
        if (readIndex >= Constants::PacketBufferCapacity) {
            readIndex = 0;
        }
    }

    void PacketBuffer::decrementReadIndex()
    {
        if (readIndex == 0) {
            readIndex = Constants::PacketBufferCapacity;
        }
        --readIndex;
    }

    void PacketBuffer::decrementWriteIndex()
    {
        if (writeIndex == 0) {
            writeIndex = Constants::PacketBufferCapacity;
        }
        --writeIndex;
    }

    size_t PacketBuffer::getReadIndex() const
    {
        return readIndex;
    }

    void PacketBuffer::setReadIndex(const size_t newReadIndex)
    {
        readIndex = newReadIndex;
    }

    bool PacketBuffer::isEmpty() const
    {
        return readIndex == writeIndex;
    }

    void PacketBuffer::clear()
    {
        readIndex = 0;
        readIndex = 0;
        writeIndex = 0;
        writeIndex = 0;
        memset(buffer->data, 0, sizeof(buffer->data));
    }

    uint8_t PacketBuffer::getFillPercent() const
    {
        return 100 * (writeIndex > readIndex ? writeIndex - readIndex : writeIndex + Constants::PacketBufferCapacity - readIndex) /
               Constants::PacketBufferCapacity;
    }
} // ananas
