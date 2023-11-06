import crypto from "crypto";

import { KitchenServer } from "utils/grpc/server.js";
import Runnable from "utils/runnable.js";
import Order from "./order.js";

class Kitchen extends KitchenServer {
	#port;
	#storagePort;
	constructor(port, operationsPath, storagePort) {
		super(port);
		this.#loadOperations(operationsPath).catch(err => {
			console.error(err);
			process.exit(-1);
		});
		this.#port = port;
		this.#storagePort = storagePort;
	}
	#operations;
	async #loadOperations(operationsPath) {
		this.#operations = (await import("../data/" + operationsPath, { assert: { type: "json" } })).default;
		this.#operations.forEach(operGroup => {
			if (!("id" in operGroup)) throw "NO ID";
			if (operGroup.id < 0) throw "wrong id!";
			if (!("operations" in operGroup)) throw "NO OPERATIONS";
			if (operGroup.operations.length === 0) throw "empty operations!";
			operGroup.operations.forEach(operation => {
				if (!("id" in operation)) throw "no op id";
				if (operation.id < 0) throw "wrong op id!";

				if (!("name" in operation)) throw "no op name";
				if (operation.name.length < 3) throw "wrong op name!";

				if (!("dependencies" in operation)) throw "no op dependencies";

				if (!("duration" in operation)) throw "no op duration";
				if (operation.duration <= 0) throw "wrong op duration!";

				if (!("equipment" in operation)) throw "no op equipment";
				if (operation.equipment < 0) throw "wrong op duration!";

				if (!("products" in operation)) throw "no op products";
				// TODO: check what is inside products
			});
		});
		console.log("Operations have been loaded succesfully");
	}

	#orders = {
		waiting: new Map(),
		cooking: new Map(),
		ready: new Map(),
	};
	async createOrder(data, callback) {
		const id = crypto.randomUUID();
		const order = new Order(id, data.request.dishIds, this.#port);
		this.#orders.waiting.set(id, order);

		await order.setup(this.#storagePort, this.#operations);
		const status = await order.validate();
		if (!status.ok) {
			await order.stop();
			this.#orders.waiting.delete(id);
			return callback(null, status);
		}
		callback(null, { ok: true, orderId: id });

		this.#orders.waiting.delete(id);
		this.#orders.cooking.set(id, order);
		order.cook().then(async food => {
			await order.stop();
			this.#orders.cooking.delete(id);
			this.#orders.ready.set(id, food);
		});
	}

	async getOrderStatus(data, callback) {
		if (this.#orders.waiting.has(data.request.id)) return callback(null, { status: "WAITING" });
		if (this.#orders.cooking.has(data.request.id))
			return callback(null, { status: "COOKING", waitTime: await this.#orders.cooking.get(data.request.id).estimateTime() });
		if (this.#orders.ready.has(data.request.id)) return callback(null, { status: "READY" });
		callback(null, { status: "UNKNOWN" });
	}

	async getOrder(data, callback) {
		if (!this.#orders.ready.has(data.request.id)) return callback(null, { ok: false });
		callback(null, { ok: true, ...this.#orders.ready.get(data.request.id) });
		this.#orders.ready.delete(data.request.id);
	}

	#cookPorts = [];
	async registerCook(data, callback) {
		this.#cookPorts.push(data.request.port);
		callback(null, {});
		this.#checkCookingRequests();
	}

	#equipmentPorts = new Map();
	async registerEquipment(data, callback) {
		if (this.#equipmentPorts.has(data.request.type)) this.#equipmentPorts.get(data.request.type).push(data.request.port);
		else this.#equipmentPorts.set(data.request.type, [data.request.port]);
		callback(null, {});
		this.#checkCookingRequests();
	}

	async checkEquipment(data, callback) {
		callback(null, { available: data.request.types.every(type => this.#equipmentPorts.get(type)?.length) });
	}

	#cookingRequests = [];
	async getCookAndEquipment(data, callback) {
		// DO
		// this.#cookingRequests.push({ equipment: data.request.equipmentType, callback });
		// this.#checkCookingRequests();
	}
	#checkCookingRequests() {
		// DO
		/*if (this.#cookPorts.length == 0) return;
		for (let i = 0; i < this.#cookingRequests.length; i++)
		{
			const equipments = this.#equipmentPorts.get(this.#cookingRequests[i].equipment) ?? [ ];
			if (equipments.length == 0) continue;
			this.#cookingRequests[i].callback({ equipmentPort: equipments.shift(), cookPort: this.#cookPorts.shift() });
			this.#cookingRequests.splice(i, 1);
			i--;
		}*/
	}
}

const kitchen = new Kitchen(process.argv[2], process.argv[3], process.argv[4]);
Runnable.childClass = undefined;
