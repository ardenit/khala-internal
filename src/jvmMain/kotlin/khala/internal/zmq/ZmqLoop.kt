package khala.internal.zmq

import co.touchlab.stately.isolate.IsolateState
import khala.internal.zmq.bindings.ZmqContext
import khala.internal.zmq.bindings.ZmqMsg
import khala.internal.zmq.bindings.ZmqSocket
import khala.internal.zmq.server.LoopState
import org.zeromq.ZPoller
import java.util.concurrent.atomic.AtomicInteger
import kotlin.concurrent.thread

private val loopCounter = AtomicInteger(0)

internal class QueueState<S>(val queue: ArrayDeque<LoopQuery<S>>, val pingSocket: ZmqSocket)

internal actual class ZmqLoop<S> actual constructor(
    context: ZmqContext,
    userStateProducer: () -> S,
    forwardListener: LoopState<S>.(String, ZmqMsg) -> Unit,
    backwardListener: LoopState<S>.(ZmqMsg) -> Unit,
    backwardRouterBindAddress: String?
) {

    internal val loopId = loopCounter.addAndGet(1)

    internal val isolatedQueue = IsolateState {
        QueueState(
            ArrayDeque<LoopQuery<S>>(),
            context.createAndBindDealer("inproc://LOOP_WORKER_$loopId")
        )
    }

    internal val poller = ZPoller(context.context)

    private val loopWorker: Thread

    init {
        isolatedQueue.access { } // wait until inproc socket binds
        val initialState = JobInitialState(
                this, context, userStateProducer,
                forwardListener, backwardListener,
                backwardRouterBindAddress
        )
        loopWorker = thread(name = "LOOP_WORKER_$loopId") {
            loopJob(initialState)
        }
    }


    actual fun invokeSafe(block: LoopState<S>.() -> Unit) {
        isolatedQueue.access {
            it.queue.addLast(InvokeQuery(block))
            val msg = ZmqMsg()
            msg.addString("")
            msg.send(it.pingSocket)
        }
    }

    actual fun stopSafe() {
        isolatedQueue.access {
            it.queue.addLast(StopQuery())
            ZmqMsg().send(it.pingSocket)
        }
    }

}
