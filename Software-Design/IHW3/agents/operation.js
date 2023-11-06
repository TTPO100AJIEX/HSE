import { CookClient, KitchenClient } from "utils/grpc/client.js";
import Runnable from "utils/runnable.js";

export default class Operation extends Runnable {
	#operation;
	#products;
	#kitchenPort;
	constructor(operation, products, kitchenPort) {
		super("./agents/operation.js", [operation, products, kitchenPort]);
		this.#operation = operation;
		this.#products = products;
		this.#kitchenPort = kitchenPort;
	}

	#cook;
	async execute() {
		// DO
		/*console.log('!');
		const kitchen = new KitchenClient(this.#kitchenPort);
		const { equipmentPort, cookPort } = await kitchen.getCookAndEquipment({ equipmentType: this.#operation.equipment });
		console.log(equipmentPort, cookPort);
		kitchen.close();*/
		return "Test";
		//this.#cook = new CookClient(cookPort);
		//return await this.#cook.cook({ operation: this.#operation, products: this.#products, equipment: equipmentPort });
	}

	async estimateTime() {
		const estimate = Math.floor(Math.random() * 10000);
		// TODO: calculate a time estimate to complete the opreation
		return estimate;
	}
}

Runnable.childClass = Operation;
