// core/fix/fast_encoder.hpp
class FastFixEncoder {
 public:
  // Writes directly to a raw memory buffer. Zero heap allocations. Zero
  // exceptions.
  static size_t build_new_order_single(char* buffer, int cl_ord_id,
                                       double price, int qty) noexcept {
    char* ptr = buffer;
    // Fast-append tags without std::string overhead
    ptr = append_tag(ptr, 35, "D");        // MsgType = NewOrderSingle
    ptr = append_tag(ptr, 11, cl_ord_id);  // ClOrdID
    ptr = append_tag(ptr, 44, price);      // Price
    ptr = append_tag(ptr, 38, qty);        // OrderQty
    return ptr - buffer;  // Returns exact byte length of the payload
  }
};
