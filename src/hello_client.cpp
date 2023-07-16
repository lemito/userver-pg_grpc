#include "hello_client.hpp"

#include <fmt/format.h>

#include <userver/yaml_config/merge_schemas.hpp>

namespace pg_grpc_service_template {

std::string HelloClient::SayHello(std::string name) {
  handlers::api::HelloRequest request;
  request.set_name(std::move(name));

  auto context = std::make_unique<grpc::ClientContext>();
  context->set_deadline(
      userver::engine::Deadline::FromDuration(std::chrono::seconds{20}));

  auto stream = client_.SayHello(request, std::move(context));

  handlers::api::HelloResponse response = stream.Finish();

  return std::move(*response.mutable_text());
}

userver::yaml_config::Schema HelloClient::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
description: >
    a user-defined wrapper around api::GreeterServiceClient that provides
    a simplified interface.
additionalProperties: false
properties:
    endpoint:
        type: string
        description: >
            Some other service endpoint (URI). 
)");
}

void AppendHelloClient(userver::components::ComponentList& component_list) {
  component_list.Append<HelloClient>();
}

}  // namespace pg_grpc_service_template
