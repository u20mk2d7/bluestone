namespace bluestone {
  struct NormalizeTick {
    uint64_t timestamp;
    uint64_t instrument_id;
    double bid_price;
    double ask_price;
    uint32_t bid_qty;
    uint32_t ask_qty;
  };

}  // namespace bluestone
