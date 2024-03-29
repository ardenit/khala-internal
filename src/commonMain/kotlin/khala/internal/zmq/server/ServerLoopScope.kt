package khala.internal.zmq.server

import khala.internal.zmq.bindings.MsgBuilder
import khala.internal.zmq.bindings.ZmqMsg
import khala.internal.zmq.bindings.buildMsg

/**
 * This class works as a receiver for lambdas that will be called from the loop, so its methods are not thread-safe.
 */
internal expect class ServerLoopScope<L> {

    fun sendMessage(msg: ZmqMsg)

    /**
     * Invokes the lambda in event loop thread after timeout.
     * Timeout is NOT exact, lambda may be called a lot later if the loop is high loaded.
     * Few seconds of delay are OK for this function.
     */
    fun invokeAfterTimeout(timeoutMillis: Long, block: ServerLoopScope<L>.(L) -> Unit)

}

internal inline fun <L> ServerLoopScope<L>.sendBackward(block: MsgBuilder.() -> Unit) {
    sendMessage(buildMsg(block))
}