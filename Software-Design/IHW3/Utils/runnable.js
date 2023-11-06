import crypto from "crypto";
import workerThreads from "worker_threads";

export default class Runnable {
	static childClass = undefined;

	#worker = null;
	#workerRequests = new Map();
	constructor(sourceFile, params = []) {
		if (Runnable.childClass && this instanceof Runnable.childClass) {
			workerThreads.parentPort.on("message", async message =>
				workerThreads.parentPort.postMessage({ id: message.id, result: await this[message.type](...message.args) })
			);
		} else {
			this.#worker = new workerThreads.Worker(sourceFile, { workerData: params });
			for (const key of Object.getOwnPropertyNames(Object.getPrototypeOf(this))) {
				this[key] = (...args) =>
					new Promise(resolve => {
						const requestId = crypto.randomUUID();
						this.#workerRequests.set(requestId, resolve);
						this.#worker.postMessage({ id: requestId, type: key, args });
					});
			}
			this.#worker.on("message", message => this.#workerRequests.get(message.id)(message.result));
		}
	}
	stop() {
		return this.#worker.terminate();
	}
}

if (!workerThreads.isMainThread) setImmediate(() => new Runnable.childClass(...workerThreads.workerData));
