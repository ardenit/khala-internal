package khala.internal.zmq

external class Buffer {

    companion object {
        fun from(str: String, encoding: String = definedExternally): Buffer
    }

}

actual typealias BinaryData = Buffer

internal actual class ZmqMsg actual constructor() {

    private val frames: ArrayDeque<BinaryData> = ArrayDeque()

    // From arguments object
    constructor(messages: dynamic) : this() {
        val length = messages.length
        for (i in 0 until length) {
            frames.addLast(messages[i])
        }
    }


    actual fun send(socket: ZmqSocket) {
        val byteBlocks = frames.toTypedArray()
        //socket.arraySender(byteBlocks)
        socket.socket.send(byteBlocks)
        close()
    }

    actual fun addBytes(bytes: BinaryData) {
        frames.addLast(bytes)
    }

    actual fun addString(str: String) {
        frames.addLast(Buffer.from(str))
    }

    actual fun popBytes(): BinaryData = frames.removeFirst()

    actual fun popString(): String = frames.removeFirst().toString()

    actual fun close() {
        frames.clear()
    }

}