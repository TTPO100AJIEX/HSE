import grpc from "@grpc/grpc-js";

import grpcClientFactory from "./client.js";

export default class grpcServerFactory {
	get(protoName, serviceName) {
		const clientClass = grpcClientFactory.get(protoName, serviceName);
		return class grpcServer extends grpc.Server {
			constructor(port) {
				super();
				this.bindAsync(`localhost:${port}`, grpc.ServerCredentials.createInsecure(), () => {
					const methods = Object.fromEntries(Object.keys(clientClass.service).map(methodName => [methodName, this[methodName].bind(this)]));
					this.addService(clientClass.service, methods);
					this.start();
					console.log("Server started");
				});
			}
		};
	}

	static get(...args) {
		return new grpcServerFactory().get(...args);
	}
}

const ManagerServer = grpcServerFactory.get("manager.proto", "manager");
const StewardServer = grpcServerFactory.get("steward.proto", "steward");
const StorageServer = grpcServerFactory.get("storage.proto", "storage");
const KitchenServer = grpcServerFactory.get("kitchen.proto", "kitchen");
const CookServer = grpcServerFactory.get("cook.proto", "cook");
const EquipmentServer = grpcServerFactory.get("equipment.proto", "equipment");
export { ManagerServer, StewardServer, StorageServer, KitchenServer, CookServer, EquipmentServer };
