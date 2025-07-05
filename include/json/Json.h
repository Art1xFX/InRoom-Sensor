#include <ArduinoJson.h>

namespace json
{
    template <size_t N>
    class Json
    {
    public:
        virtual void toJsonVariant(JsonObject &jsonObject) const = 0;

        virtual void fromJsonVariant(const JsonObject &jsonObject) = 0;

        size_t toJsonString(char *jsonString) const;

        DeserializationError fromJsonString(const char *jsonString);
    };

    template <size_t N>
    size_t Json<N>::toJsonString(char *jsonString) const
    {
        JsonDocument document;
        JsonObject jsonObject = document.to<JsonObject>();
        toJsonVariant(jsonObject);
        return serializeJson(document, jsonString, N);
    }

    template <size_t N>
    DeserializationError Json<N>::fromJsonString(const char *jsonString)
    {
        JsonDocument document;
        DeserializationError error = deserializeJson(document, jsonString);
        if (error)
        {
            return error;
        }
        JsonObject jsonObject = document.as<JsonObject>();
        fromJsonVariant(jsonObject);
        return error;
    }
}