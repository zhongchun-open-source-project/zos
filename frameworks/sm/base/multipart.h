// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_MULTIPART_H_
#define SM_BASE_MULTIPART_H_

#include <string>
#include <vector>

#include "sm/base/sigslot.h"
#include "sm/base/stream.h"

namespace sm {

///////////////////////////////////////////////////////////////////////////////
// MultipartStream - Implements an RFC2046 multipart stream by concatenating
// the supplied parts together, and adding the correct boundaries.
///////////////////////////////////////////////////////////////////////////////

class MultipartStream : public StreamInterface, public sigslot::has_slots<> {
 public:
  MultipartStream(const std::string& type, const std::string& boundary);
  ~MultipartStream() override;

  void GetContentType(std::string* content_type);

  // Note: If content_disposition and/or content_type are the empty string,
  // they will be omitted.
  bool AddPart(StreamInterface* data_stream,
               const std::string& content_disposition,
               const std::string& content_type);
  bool AddPart(const std::string& data,
               const std::string& content_disposition,
               const std::string& content_type);
  void EndParts();

  // Calculates the size of a part before actually adding the part.
  size_t GetPartSize(const std::string& data,
                     const std::string& content_disposition,
                     const std::string& content_type) const;
  size_t GetEndPartSize() const;

  // StreamInterface
  StreamState GetState() const override;
  StreamResult Read(void* buffer,
                    size_t buffer_len,
                    size_t* read,
                    int* error) override;
  StreamResult Write(const void* data,
                     size_t data_len,
                     size_t* written,
                     int* error) override;
  void Close() override;
  bool SetPosition(size_t position) override;
  bool GetPosition(size_t* position) const override;
  bool GetSize(size_t* size) const override;
  bool GetAvailable(size_t* size) const override;

 private:
  typedef std::vector<StreamInterface*> PartList;

  // StreamInterface Slots
  void OnEvent(StreamInterface* stream, int events, int error);

  std::string type_, boundary_;
  PartList parts_;
  bool adding_;
  size_t current_;  // The index into parts_ of the current read position.
  size_t position_;  // The current read position in bytes.

  SM_DISALLOW_COPY_AND_ASSIGN(MultipartStream);
};

}  // namespace sm

#endif  // SM_BASE_MULTIPART_H_
