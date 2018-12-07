// This file is part of the `Selene` library.
// Copyright 2017-2018 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#ifndef SELENE_IMG_DYNAMIC_DYN_IMAGE_VIEW_HPP
#define SELENE_IMG_DYNAMIC_DYN_IMAGE_VIEW_HPP

/// @file

#include <selene/img/dynamic/DynImageIterators.hpp>
#include <selene/img/dynamic/UntypedLayout.hpp>

#include <cstring>
#include <type_traits>

namespace sln {

template <ImageModifiability modifiability_ = ImageModifiability::Constant>
class DynImageView
{
public:
  using DataPtrType = typename DataPtr<modifiability_>::Type;
  template <typename PixelType> using PixelTypePtr =
      std::conditional_t<modifiability_ == ImageModifiability::Constant, const PixelType*, PixelType*>;

  template <typename PixelType> using iterator = DynImageRowIterator<PixelType, modifiability_>;  ///< The iterator type.
  template <typename PixelType> using const_iterator = ConstDynImageRowIterator<PixelType, modifiability_>;  ///< The const_iterator type.

  constexpr static bool is_view = true;
  constexpr static bool is_owning = false;
  constexpr static bool is_modifiable = (modifiability_ == ImageModifiability::Mutable);

  constexpr static ImageModifiability modifiability()
  {
    return modifiability_;
  }

  DynImageView() = default;
  DynImageView(
      DataPtr<modifiability_> ptr,
      UntypedLayout layout,
      UntypedImageSemantics semantics = UntypedImageSemantics{});

  const UntypedLayout& layout() const noexcept;
  const UntypedImageSemantics& semantics() const noexcept;

  PixelLength width() const noexcept;
  PixelLength height() const noexcept;
  std::int16_t nr_channels() const noexcept;
  std::int16_t nr_bytes_per_channel() const noexcept;
  Stride stride_bytes() const noexcept;
  std::ptrdiff_t row_bytes() const noexcept;
  std::ptrdiff_t total_bytes() const noexcept;
  PixelFormat pixel_format() const noexcept;
  SampleFormat sample_format() const noexcept;

  bool is_packed() const noexcept;
  bool is_empty() const noexcept;
  bool is_valid() const noexcept;

  template <typename PixelType> iterator<PixelType> begin() noexcept;
  template <typename PixelType> const_iterator<PixelType> begin() const noexcept;
  template <typename PixelType> const_iterator<PixelType> cbegin() const noexcept;

  template <typename PixelType> iterator<PixelType> end() noexcept;
  template <typename PixelType> const_iterator<PixelType> end() const noexcept;
  template <typename PixelType> const_iterator<PixelType> cend() const noexcept;

  DataPtrType byte_ptr() const noexcept;
  DataPtrType byte_ptr(PixelIndex y) const noexcept;
  DataPtrType byte_ptr(PixelIndex x, PixelIndex y) const noexcept;

  template <typename PixelType> PixelTypePtr<PixelType> data() const noexcept;
  template <typename PixelType> PixelTypePtr<PixelType> data(PixelIndex y) const noexcept;
  template <typename PixelType> PixelTypePtr<PixelType> data_row_end(PixelIndex y) const noexcept;
  template <typename PixelType> PixelTypePtr<PixelType> data(PixelIndex x, PixelIndex y) const noexcept;

  template <typename PixelType,
            typename T = void,
            typename = std::enable_if_t<modifiability_ == ImageModifiability::Mutable, T>>
  PixelType& pixel(PixelIndex x, PixelIndex y) noexcept;

  template <typename PixelType>
  const PixelType& pixel(PixelIndex x, PixelIndex y) const noexcept;

  DynImageView<modifiability_>& view() noexcept;
  DynImageView<ImageModifiability::Constant> view() const noexcept;
  DynImageView<ImageModifiability::Constant> constant_view() const noexcept;

  void clear();

private:
  DataPtr<modifiability_> ptr_;
  UntypedLayout layout_;
  UntypedImageSemantics semantics_;

  sln::Bytes compute_data_offset(PixelIndex y) const noexcept;
  sln::Bytes compute_data_offset(PixelIndex x, PixelIndex y) const noexcept;
};

using MutableDynImageView = DynImageView<ImageModifiability::Mutable>;
using ConstantDynImageView = DynImageView<ImageModifiability::Constant>;

template <ImageModifiability modifiability_0, ImageModifiability modifiability_1>
bool equal(const DynImageView<modifiability_0>& dyn_img_0, const DynImageView<modifiability_1>& dyn_img_1);

// ----------
// Implementation:

template <ImageModifiability modifiability_>
DynImageView<modifiability_>::DynImageView(
    DataPtr<modifiability_> ptr,
    UntypedLayout layout,
    UntypedImageSemantics semantics)
    : ptr_(ptr), layout_(layout), semantics_(semantics)
{
}

template <ImageModifiability modifiability_>
const UntypedLayout& DynImageView<modifiability_>::layout() const noexcept
{
  return layout_;
}

template <ImageModifiability modifiability_>
const UntypedImageSemantics& DynImageView<modifiability_>::semantics() const noexcept
{
  return semantics_;
}

template <ImageModifiability modifiability_>
PixelLength DynImageView<modifiability_>::width() const noexcept
{
  return layout_.width;
}

template <ImageModifiability modifiability_>
PixelLength DynImageView<modifiability_>::height() const noexcept
{
  return layout_.height;
}

template <ImageModifiability modifiability_>
std::int16_t DynImageView<modifiability_>::nr_channels() const noexcept
{
  return layout_.nr_channels;
}

template <ImageModifiability modifiability_>
std::int16_t DynImageView<modifiability_>::nr_bytes_per_channel() const noexcept
{
  return layout_.nr_bytes_per_channel;
}

template <ImageModifiability modifiability_>
Stride DynImageView<modifiability_>::stride_bytes() const noexcept
{
  return layout_.stride_bytes;
}

template <ImageModifiability modifiability_>
std::ptrdiff_t DynImageView<modifiability_>::row_bytes() const noexcept
{
  return layout_.row_bytes();
}

template <ImageModifiability modifiability_>
std::ptrdiff_t DynImageView<modifiability_>::total_bytes() const noexcept
{
  return layout_.total_bytes();
}

template <ImageModifiability modifiability_>
PixelFormat DynImageView<modifiability_>::pixel_format() const noexcept
{
  return semantics_.pixel_format;
}

template <ImageModifiability modifiability_>
SampleFormat DynImageView<modifiability_>::sample_format() const noexcept
{
  return semantics_.sample_format;
}

template <ImageModifiability modifiability_>
bool DynImageView<modifiability_>::is_packed() const noexcept
{
  return layout_.is_packed();
}

template <ImageModifiability modifiability_>
bool DynImageView<modifiability_>::is_empty() const noexcept
{
  return ptr_.data() == nullptr || layout_.width == 0 || layout_.height == 0;
}

template <ImageModifiability modifiability_>
bool DynImageView<modifiability_>::is_valid() const noexcept
{
  return !is_empty();
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::begin() noexcept -> iterator<PixelType>
{
  return DynImageRowIterator<PixelType, modifiability_>(DynImageRow<PixelType, modifiability_>(this, 0_idx));
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::begin() const noexcept -> const_iterator<PixelType>
{
  return ConstDynImageRowIterator<PixelType, modifiability_>(
      ConstDynImageRow<PixelType, modifiability_>(
          this,
          0_idx));
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::cbegin() const noexcept -> const_iterator<PixelType>
{
  return ConstDynImageRowIterator<PixelType, modifiability_>(
      ConstDynImageRow<PixelType, modifiability_>(
          this,
          0_idx));
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::end() noexcept -> iterator<PixelType>
{
  return DynImageRowIterator<PixelType, modifiability_>(
      DynImageRow<PixelType, modifiability_>(
          this,
          PixelIndex{this->height()}));
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::end() const noexcept -> const_iterator<PixelType>
{
  return ConstDynImageRowIterator<PixelType, modifiability_>(
      ConstDynImageRow<PixelType, modifiability_>(
          this,
          PixelIndex{this->height()}));
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::cend() const noexcept -> const_iterator<PixelType>
{
  return ConstDynImageRowIterator<PixelType, modifiability_>(
      ConstDynImageRow<PixelType, modifiability_>(
          this,
          PixelIndex{this->height()}));
}

template <ImageModifiability modifiability_>
auto DynImageView<modifiability_>::byte_ptr() const noexcept -> DataPtrType
{
  return ptr_.data();
}

template <ImageModifiability modifiability_>
auto DynImageView<modifiability_>::byte_ptr(PixelIndex y) const noexcept -> DataPtrType
{
  return ptr_.data() + this->compute_data_offset(y);
}

template <ImageModifiability modifiability_>
auto DynImageView<modifiability_>::byte_ptr(PixelIndex x, PixelIndex y) const noexcept -> DataPtrType
{
  return ptr_.data() + this->compute_data_offset(x, y);
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::data() const noexcept -> PixelTypePtr<PixelType>
{
  return reinterpret_cast<PixelTypePtr<PixelType>>(this->byte_ptr());
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::data(PixelIndex y) const noexcept -> PixelTypePtr<PixelType>
{
  return reinterpret_cast<PixelTypePtr<PixelType>>(this->byte_ptr(y));
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::data_row_end(PixelIndex y) const noexcept -> PixelTypePtr<PixelType>
{
  return reinterpret_cast<PixelTypePtr<PixelType>>(this->byte_ptr(y) + layout_.nr_bytes_per_pixel() * layout_.width);
}

template <ImageModifiability modifiability_>
template <typename PixelType>
auto DynImageView<modifiability_>::data(PixelIndex x, PixelIndex y) const noexcept -> PixelTypePtr<PixelType>
{
  return reinterpret_cast<PixelTypePtr<PixelType>>(this->byte_ptr(x, y));
}

template <ImageModifiability modifiability_>
template <typename PixelType, typename, typename>
PixelType& DynImageView<modifiability_>::pixel(PixelIndex x, PixelIndex y) noexcept
{
  return *data<PixelType>(x, y);
}

template <ImageModifiability modifiability_>
template <typename PixelType>
const PixelType& DynImageView<modifiability_>::pixel(PixelIndex x, PixelIndex y) const noexcept
{
  return *data<PixelType>(x, y);
}

template <ImageModifiability modifiability_>
DynImageView<modifiability_>& DynImageView<modifiability_>::view() noexcept
{
  return *this;
}

template <ImageModifiability modifiability_>
DynImageView<ImageModifiability::Constant> DynImageView<modifiability_>::view() const noexcept
{
  // TODO: optimize
  return constant_view();
}

template <ImageModifiability modifiability_>
DynImageView<ImageModifiability::Constant> DynImageView<modifiability_>::constant_view() const noexcept
{
  // TODO: optimize
  return DynImageView<ImageModifiability::Constant>{this->byte_ptr(), this->layout(), this->semantics()};
}

template <ImageModifiability modifiability_>
void DynImageView<modifiability_>::clear()
{
  ptr_ = DataPtr<modifiability_>{};
  layout_ = UntypedLayout{};
  semantics_ = UntypedImageSemantics{};
}

template <ImageModifiability modifiability_>
sln::Bytes DynImageView<modifiability_>::compute_data_offset(PixelIndex y) const noexcept
{
  return sln::Bytes{layout_.stride_bytes * y};
}

template <ImageModifiability modifiability_>
sln::Bytes DynImageView<modifiability_>::compute_data_offset(PixelIndex x, PixelIndex y) const noexcept
{
  return sln::Bytes{layout_.stride_bytes * y + layout_.nr_bytes_per_pixel() * x};
}

template <ImageModifiability modifiability_0, ImageModifiability modifiability_1>
bool equal(const DynImageView<modifiability_0>& dyn_img_0, const DynImageView<modifiability_1>& dyn_img_1)
{
  // Special case: if both images have a zero-length side, the shall be considered equal (both are invalid)
  if ((dyn_img_0.width() == 0 || dyn_img_0.height() == 0) && (dyn_img_1.width() == 0 || dyn_img_1.height() == 0))
  {
    return true;
  }

  if (dyn_img_0.width() != dyn_img_1.width() || dyn_img_0.height() != dyn_img_1.height())
  {
    return false;
  }

  for (auto y = 0_idx; y < dyn_img_0.height(); ++y)
  {
    const auto end0 = dyn_img_0.byte_ptr(static_cast<PixelIndex>(dyn_img_0.width()), y);
    const auto begin0 = dyn_img_0.byte_ptr(y);
    const auto begin1 = dyn_img_1.byte_ptr(y);

    // std::equal may not be optimized to std::memcmp, even though we're dealing with a POD-type here...
    // const bool equal_row = std::equal(begin0, end0, begin1);
    // ...so let's just call std::memcmp directly:
    const auto nr_bytes = std::distance(begin0, end0);
    const bool equal_row = (std::memcmp(begin0, begin1, nr_bytes) == 0);

    if (!equal_row)
    {
      return false;
    }
  }

  return true;
}

}  // namespace sln

#endif  // SELENE_IMG_DYNAMIC_DYN_IMAGE_VIEW_HPP
