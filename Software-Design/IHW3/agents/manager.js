import { KitchenClient, StewardClient } from "utils/grpc/client.js";
import { ManagerServer } from "utils/grpc/server.js";

class Manager extends ManagerServer {
	#steward;
	#kitchen;
	constructor(port, stewardPort, kitchenPort) {
		super(port);
		this.#steward = new StewardClient(stewardPort);
		this.#kitchen = new KitchenClient(kitchenPort);
	}

	#menuRequest = null;
	async getMenu(data, callback) {
		if (this.#menuRequest) return callback(null, await this.#menuRequest);
		this.#menuRequest = this.#steward.getMenu({});
		callback(null, await this.#menuRequest);
		this.#menuRequest = null;
	}

	async placeOrder(data, callback) {
		callback(null, await this.#kitchen.createOrder(data.request));
	}

	async getOrderStatus(data, callback) {
		callback(null, await this.#kitchen.getOrderStatus(data.request));
	}

	async getOrder(data, callback) {
		callback(null, await this.#kitchen.getOrder(data.request));
	}
}

const manager = new Manager(process.argv[2], process.argv[3], process.argv[4]);
