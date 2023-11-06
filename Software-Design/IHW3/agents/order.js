import { KitchenClient, StorageClient } from "utils/grpc/client.js";
import Runnable from "utils/runnable.js";
import Dish from "./dish.js";

export default class Order extends Runnable {
	#id;
	#order;
	#kitchenPort;
	constructor(id, order, kitchenPort) {
		super("./agents/order.js", [id, order, kitchenPort]);
		this.#id = id;
		this.#order = order;
		this.#kitchenPort = kitchenPort;
	}

	#storagePort;
	#operations;
	async setup(storagePort, operations) {
		this.#storagePort = storagePort;
		this.#operations = operations;
	}

	#products;
	async validate() {
		const kitchen = new KitchenClient(this.#kitchenPort);
		const equipment = await kitchen.checkEquipment({ types: [] }); // TODO: fill list of equipment types needed for this order (-1s should not be included)
		kitchen.close();
		if (!equipment.available) return { ok: false, error: "Not enough equipment" };

		const storage = new StorageClient(this.#storagePort);
		const productList = { products: [] }; // TODO: fill productList from the received order (check operations, get list of products, etc.)
		const { ok, products } = await storage.getProducts(productList);
		storage.close();
		if (!ok) return { ok: false, error: "Not enough products" };
		this.#products = products;
		return { ok: true };
	}

	#dishes;
	async cook() {
		this.#dishes = this.#order.map(dishId => {
			const products = []; // TODO: take products needed for this dish from this.#products and put them into this array
			const { operations } = this.#operations.find(dish => dish.id == dishId);
			return new Dish(operations, products, this.#kitchenPort);
		});
		const food = await Promise.all(this.#dishes.map(dish => dish.cook()));
		await Promise.all(this.#dishes.map(dish => dish.stop()));
		return { food };
	}

	async estimateTime() {
		const dishEstimates = await Promise.all(this.#dishes.map(dish => dish.estimateTime()));
		return Math.max(...dishEstimates);
	}
}

Runnable.childClass = Order;
