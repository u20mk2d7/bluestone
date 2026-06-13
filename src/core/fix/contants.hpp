// core/fix/constants.hpp
namespace bluestone::fix {
  constexpr int Tag_MsgType = 35;
  constexpr int Tag_SenderCompID = 49;
  constexpr int Tag_TargetCompID = 56;
  constexpr int Tag_Price = 44;

  enum class MsgType : char {
    Logon = 'A',
    MarketDataRequest = 'V',
    MarketDataSnapshot = 'W',
    NewOrderSingle = 'D'
  };
}  // namespace bluestone::fix
