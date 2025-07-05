#include <ArduinoJson.h>

namespace json
{
    template <size_t N>
    class Json
    {
    public:
        virtual void toJsonVariant(JsonVariant &jsonVariant) const = 0;

        virtual void fromJsonVariant(const JsonVariant &jsonVariant) = 0;

        size_t toJsonString(char *jsonString) const;

        DeserializationError fromJsonString(const char *jsonString);
    };

    template <size_t N>
    size_t Json<N>::toJsonString(char *jsonString) const
    {
        JsonDocument document;
        JsonVariant jsonVariant = document.to<JsonVariant>();
        toJsonVariant(jsonVariant);
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
        JsonVariant jsonVariant = document.as<JsonVariant>();
        fromJsonVariant(jsonVariant);
        return error;
    }
}