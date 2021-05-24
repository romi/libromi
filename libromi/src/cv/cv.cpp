/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#include <math.h>
#include <iostream>
#include <map>
#include <r.h>
#include "cv/cv.h"
#include "slic.h"
#include "cv/connected.h"

namespace romi {

        static inline bool is_black(float color)
        {
                return color == 0.0f;
        }

        bool segment_crosses_white_area(const Image& mask, v3 start, v3 end)
        {
                size_t max_count = 0;
                size_t count = 0;
                enum State { kOut, kIn };
                State state = kOut;

                double d = (end - start).norm();
                size_t n = 1 + (size_t) ceil(d);
                
                for (size_t i = 0; i <= n; i++) {
                        v3 p = start + (end - start) * (double) i / (double) n;
                        float color = mask.get(0, (size_t) p.x(), (size_t) p.y());

                        if (is_black(color)) {
                                if (state == kIn) {
                                        state = kOut;
                                        if (count > max_count)
                                                max_count = count;
                                } else {
                                        // normal case. skip.
                                }
                        } else { // white
                                if (state == kOut) {
                                        state = kIn;
                                        count = 1;
                                } else {
                                        count++;
                                }
                        }
                }
                
                return max_count > 0;
        }

        void filter_mask(Image &in, Image &out, size_t n)
        {
                Image eroded;
                in.erode(n, eroded);
                eroded.dilate(n, out);
        }

        static void convert_32bit_rgb(Image &mask, uint32_t *buf)
        {
                size_t len = mask.width() * mask.height();
                auto data = mask.data();
                for (size_t i = 0; i < len; i++) {
                        float grey_f = data[i];
                        uint32_t grey_i = (uint32_t) (255.0f * grey_f);
                        buf[i] = (grey_i << 16) | (grey_i << 8) | grey_i;
                }
        }

        // PH This vector of pairs is stored X, Y. It should be a struct
        // really to make it obvious.
        static Centers calculate_centers(const std::vector<double> &kseedsl,
                                         const std::vector<double> &kseedsa,
                                         const std::vector<double> &kseedsb,
                                         const std::vector<double> &kseedsx,
                                         const std::vector<double> &kseedsy)
        {
                // XY ORDER
                Centers centres;
        
                for (size_t xyindex = 0; xyindex < kseedsx.size(); xyindex++) {

                        // PH IMPORTANT: if you uncomment this if statement it
                        // will not plot segments that border a white area.
                        // This is due to colours at a border containing non 0
                        // colour info.
                
                        // if((kseedsl[xyindex] == 0) && (kseedsa[xyindex] == 0) && (kseedsb[xyindex] == 0))
                
                        if ((kseedsl[xyindex] < 1.0)
                            && (kseedsa[xyindex] < 1.0)
                            && (kseedsb[xyindex] < 1.0))
                        
                                centres.push_back(Center((uint32_t) kseedsx[xyindex],
                                                         (uint32_t) kseedsy[xyindex]));
                }
                return centres;
        }

        Centers calculate_centers(Image &mask, size_t max_centers)
        {
                size_t len = mask.width() * mask.height();
                std::vector<u_int32_t> ubuff(len, 0);
                convert_32bit_rgb(mask, ubuff.data());

                std::vector<int> segmentation_labels;
                size_t segmentation_size = mask.height() * mask.width();
                segmentation_labels.resize(segmentation_size);

                int numlabels(0);

                // This is the number regions * 10 (initial)
                // e.g. segmentation_size/2250 = 225 segments.
                //const int spcount = segmentation_size/2250;

                //   const double compactness = 20.0;
                const double compactness = 80.0;
                std::vector<double> kseedsl(0);
                std::vector<double> kseedsa(0);
                std::vector<double> kseedsb(0);
                std::vector<double> kseedsx(0);
                std::vector<double> kseedsy(0);

                SLIC slic;
                slic.DoSuperpixelSegmentation_ForGivenNumberOfSuperpixels(ubuff.data(),
                                                                          static_cast<int>(mask.width()),
                                                                          static_cast<int>(mask.height()),
                                                                          segmentation_labels, numlabels,
                                                                          (int) max_centers,
                                                                          compactness,
                                                                          kseedsl, kseedsa,
                                                                          kseedsb, kseedsx,
                                                                          kseedsy);
                
                std::cout << "number unique segments (slic) = "
                          << kseedsl.size() << std::endl;

                // Use Centres from algo
                return calculate_centers(kseedsl, kseedsa, kseedsb, kseedsx, kseedsy);
        }
        
        std::vector<Centers> sort_centers(Centers& centers, Image& components)
        {
                using LabelMap = std::map<size_t,size_t>;
                using Label = std::pair<size_t,size_t>;
                
                std::vector<Centers> list;
                LabelMap label_to_index;
                
                for (auto center: centers) {
                        float color = components.get(0, center.first, center.second);
                        size_t label = (size_t) (color * 255.0f); // ???
                        
                        LabelMap::const_iterator pos = label_to_index.find(label);
                        if (pos == label_to_index.end()) {
                                size_t index = list.size();
                                Centers v;
                                v.emplace_back(center);
                                list.emplace_back(v);
                                label_to_index.insert(Label(label, index));
                        } else {
                                size_t index = pos->second;
                                list[index].emplace_back(center);
                        }
                }

                r_debug("Found %zu sets of centers", list.size());
                
                return list;
        }

        std::vector<std::pair<uint32_t, uint32_t>>
        calculate_adjacent_centres(const std::vector<double> &kseedsl,
                                   const std::vector<double> &kseedsx,
                                   const std::vector<double> &kseedsy)
        {
                using Center = std::pair<uint32_t, uint32_t>;
                // XY ORDER
                std::vector<Center> centres;
                for (size_t xyindex = 0; xyindex < kseedsx.size(); xyindex++) {
                        if ((kseedsl[xyindex] > 0) && (kseedsl[xyindex] < 1)) {
                                centres.emplace_back((uint32_t) kseedsx[xyindex],
                                                     (uint32_t) kseedsy[xyindex]);
                        }
                }
                return centres;
        }

        void compute_connected_components(Image &mask, Image &components)
        {
            std::vector<uint8_t> label_data_(mask.data().size());
            ConnectedComponents cc_(30);
            std::vector<unsigned int> out_data(mask.width() * mask.height());
            
            cc_.connected(mask.export_byte_data().data(), out_data.data(),
                          (int) mask.width(), (int) mask.height(),
                          std::equal_to<unsigned char>(),
                          constant<bool,true>());
            std::copy(out_data.data(),
                      out_data.data() + out_data.size() ,
                      label_data_.data());
            components.import(mask.type(), label_data_.data(),
                              mask.width(), mask.height());

        }
}
