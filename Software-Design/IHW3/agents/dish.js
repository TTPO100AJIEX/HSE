import Runnable from "utils/runnable.js";
import Operation from "./operation.js";

export default class Dish extends Runnable {
	#operations;
	#products;
	#kitchenPort;
	constructor(operations, products, kitchenPort) {
		super("./agents/dish.js", [operations, products, kitchenPort]);
		this.#operations = operations;
		this.#products = products;
		this.#kitchenPort = kitchenPort;
	}

	#processes;
	async cook() {
		// TODO: actually redo this entire function
		// TODO: use order of operations (dependencies)
		this.#processes = this.#operations.map(operation => {
			const products = []; // TODO: take products needed for this operation from this.#products and put them into this array
			return new Operation(operation, products, this.#kitchenPort);
		});
		const dish = await Promise.all(this.#processes.map(operation => operation.execute()));
		await Promise.all(this.#processes.map(operation => operation.stop()));
		return dish.join("+");
	}

	async estimateTime() {
		const estimate = Math.floor(Math.random() * 10000);
		// TODO: query this.#processes and calculate a time estimate to cook this dish
		return estimate;
	}
}

Runnable.childClass = Dish;
