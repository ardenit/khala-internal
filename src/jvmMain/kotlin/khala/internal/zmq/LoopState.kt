@file:JvmName("LoopStateActualKt")
package khala.internal.zmq

import khala.internal.zmq.server.LoopState
import org.zeromq.ZPoller

internal actual fun <S> LoopState<S>.sendForward(address: String, msg: khala.internal.zmq.bindings.ZmqMsg) {
    val socket = forwardSockets[address] ?: run {
        val newSocket = context.createAndConnectDealer(address)
        forwardSockets[address] = newSocket
        loop.poller.register(newSocket.socket, { _, _ ->
            this.forwardListener(address, ZmqMsg.recv(newSocket)!!)
            true
        }, ZPoller.POLLIN)
        newSocket
    }
    msg.send(socket)
}

internal actual fun <S> LoopState<S>.remove(address: String) {
    forwardSockets.remove(address)?.let {
        loop.poller.unregister(it.socket)
        it.close()
    }
}