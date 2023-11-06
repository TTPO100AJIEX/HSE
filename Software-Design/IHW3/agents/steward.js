import getDishProducts from "utils/getDishProducts.js";
import { StorageClient } from "utils/grpc/client.js";
import { StewardServer } from "utils/grpc/server.js";

class Steward extends StewardServer {
	#storage;
	constructor(port, menuPath, storagePort) {
		super(port);
		this.#loadMenu(menuPath).catch(err => {
			console.error(err);
			process.exit(-1);
		});
		this.#storage = new StorageClient(storagePort);
	}

	#menu;
	async #loadMenu(menuPath) {
		this.#menu = (await import("../data/" + menuPath, { assert: { type: "json" } })).default;
		this.#menu.forEach(dish => {
			if (!("id" in dish)) throw "NO ID";
			if (dish.id < 0) throw "wrong id!";

			if (!("price" in dish)) throw "NO PRICE";
			if (dish.price < 0) throw "wrong price";

			if (!("name" in dish)) throw "no name";

			if (dish.name.length === 0) throw "empty name";

			if (!("description" in dish)) throw "no description";
			if (!("operations" in dish)) throw "NO OPERATIONS";

			dish.operations.forEach(operation => {
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

				operation.products.forEach(product => {
					if (!("type" in product)) throw "no prod type";
					if (product.type < 0) throw "bad type";
					if (!("quantity" in product)) throw "no prod type";
					if (product.quantity < 0) throw "bad quantity";
				});
			});

			dish.available = 1;
		});
		console.log("Menu has been loaded succesfully");
	}
	async #updateMenu() {
		const { products } = await this.#storage.getAvailable({});
		this.#menu.forEach(dish => {
			getDishProducts(dish).forEach(product => {
				if (products.map(el => el.type).includes(product.type)) {
					if (products.find(prod => prod.type === product.type).quantity < product.quantity) dish.available = 0;
				} else {
					// TODO: maybe available = 0 should also be here?
					console.log(`Product ${product.type} is unavailable`);
				}
				/* maybe like this?
				const productObject = products.find(prod => prod.type === product.type);
				if (!productObject || productObject.quantity < product.quantity) dish.available = 0;
				else dish.available = 1;
				 */
			});
		});
		// TODO: what if new products appeared?
	}

	async getMenu(data, callback) {
		await this.#updateMenu();
		callback(null, { dishes: this.#menu.filter(dish => dish.available) });
	}
}

const steward = new Steward(process.argv[2], process.argv[3], process.argv[4]);
