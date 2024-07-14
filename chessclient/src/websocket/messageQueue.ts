import Message from './message';

/**
 * Queue used to handle messages.
 */
export default class MessageQueue {
    private queue: Message[] = [];

    /**
     * Add a message to the queue.
     * 
     * @param message The message to be added to the queue
     */
    enqueue(message: Message): void {
        this.queue.push(message);
    }

    /**
     * Retrieve a message from the queue.
     * 
     * @returns The last message in the queue.
     */
    dequeue(): Message {
        return this.queue.shift();
    }

    /**
     * Determine if the message queue is empty.
     * 
     * @returns True if the queue is empty, false otherwise
     */
    isEmpty(): boolean {
        return this.queue.length == 0;
    }
}