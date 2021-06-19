
class IEncoder
{
public:
        virtual ~IEncoder() = default;

        virtual void setup() = 0;
        virtual long get_value() = 0;
        virtual double get_speed() = 0;
};
