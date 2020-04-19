#pragma once

#include <vector>
#include <sstream>
using namespace std;

#include <boost/shared_ptr.hpp>

namespace marketdata
{
  namespace curve
  {
    enum InterpolationType
    {
      NONE_INTERPOLATION,
      FLAT_INTERPOLATION,
      LINEAR_INTERPOLATION,
      SPLINE_INTERPOLATION
    };

    ostream& operator>> (ostream& os, const InterpolationType& interpolation_type);

    enum ExtrapolationType
    {
      NONE_EXTRAPOLATION,
      FLAT_EXTRAPOLATION
    };

    ostream& operator>> (ostream& os, const ExtrapolationType& extrapolation_type);

    enum CurveType
    {
      DISCOUNTING,
      FORECASTING_MONTLY,
      FORECASTING_QUARTERLY,
      FORECASTING_SEMIANNUALY,
      FORECASTING_YEARLY
    };

    ostream& operator>> (ostream& os, const CurveType& curve_type);

    template <class X, class Y>
    class Interpolation2D
    {
    protected:
      vector<X> m_x_values;
      vector<Y> m_y_values;
      InterpolationType m_type;

    public:
      // constructors
      Interpolation2D(vector<X> x_values, vector<Y> y_values, InterpolationType type = NONE_INTERPOLATION)
        : m_x_values(x_values), m_y_values(y_values), m_type(type)
      {
      }

      // utilities
      void reset_y_values(const vector<Y>& y_values);
      bool check_valid(X x_value) const;
      virtual Y get_value(X x_value) const
      {
        ostringstream error_string;
        error_string << "Interpolation2D::get_value() is not available";
        std::exception(error_string.str().c_str());

        return Y(); // will not come here at all
      }

      inline InterpolationType get_type() const
      {
        return m_type;
      }
    };

    template <class X, class Y>
    class Extrapolation2D
    {
    protected:
      vector<X> m_x_values;
      vector<Y> m_y_values;
      ExtrapolationType m_type;

    public:
      // constructors
      Extrapolation2D(vector<X> x_values, vector<Y> y_values, ExtrapolationType type = NONE_EXTRAPOLATION)
        : m_x_values(x_values), m_y_values(y_values), m_type(type)
      {
      }

      // utilities
      void reset_y_values(const vector<Y>& y_values);
      bool check_valid(X x_value) const;
      virtual Y get_value(X x_value) const
      {
        ostringstream error_string;
        error_string << "Extrapolation2D::get_value() is not available";
        std::exception(error_string.str().c_str());

        return Y(); // will not come here at all
      }

      inline ExtrapolationType get_type() const
      {
        return m_type;
      }
    };

    template <class X, class Y>
    class InterpolationExtrapolation2D
    {
    private:
      InterpolationType m_interpolation_type;
      ExtrapolationType m_extrapolation_type;

      boost::shared_ptr<Interpolation2D<X,Y> > m_interpolator;
      boost::shared_ptr<Extrapolation2D<X,Y> > m_extrapolator;
    public:
      // Constructor
      InterpolationExtrapolation2D(vector<X> x_values, vector<Y> y_values, InterpolationType interp_type, ExtrapolationType extrp_type);

      // utilities
      void reset_y_values(const vector<Y>& y_values);
      Y get_value(X x_value) const;
      InterpolationType get_interpolation() const;
      ExtrapolationType get_extrapolation() const;
    };

    template <class X, class Y>
    class Flat_Interpolation2D : public Interpolation2D<X,Y>
    {
    public:
      // constructors
      Flat_Interpolation2D(vector<X> x_values, vector<Y> y_values)
        : Interpolation2D<X,Y>(x_values, y_values, FLAT_INTERPOLATION)
      {
      }

      // utilities
      virtual Y get_value(X x_value) const;

    };

    template <class X, class Y>
    class Linear_Interpolation2D : public Interpolation2D<X, Y>
    {
    public:
      // constructors
      Linear_Interpolation2D(vector<X> x_values, vector<Y> y_values)
        : Interpolation2D<X,Y>(x_values, y_values, LINEAR_INTERPOLATION)
      {
      }

      // utilities
      virtual Y get_value(X x_value) const;
    };

    template <class X, class Y>
    class Flat_Extrapolation2D : public Extrapolation2D<X, Y>
    {
    public:
      // constructors
      Flat_Extrapolation2D(vector<X> x_values, vector<Y> y_values)
        : Extrapolation2D<X,Y>(x_values, y_values, FLAT_EXTRAPOLATION)
      {
      }

      // utilities
      virtual Y get_value(X x_value) const;
    };

    template<class X, class Y>
    inline void Interpolation2D<X, Y>::reset_y_values(const vector<Y>& y_values)
    {
      assert(m_y_values.size() == y_values.size());
      m_y_values = y_values;
    }

    template<class X, class Y>
    inline bool Interpolation2D<X, Y>::check_valid(X x_value) const
    {
      if (this->m_x_values.empty())
      {
        return false;
      }

      if (x_value < this->m_x_values[0])
      {
        return false;
      }
      else if (this->m_x_values[this->m_x_values.size() - 1] < x_value)
      {
        return false;
      }

      return true;
    }

    template<class X, class Y>
    inline void Extrapolation2D<X, Y>::reset_y_values(const vector<Y>& y_values)
    {
      assert(m_y_values.size() == y_values.size());
      m_y_values = y_values;
    }

    template<class X, class Y>
    inline bool Extrapolation2D<X, Y>::check_valid(X x_value) const
    {
      if (this->m_x_values.empty())
      {
        return false;
      }

      if (x_value < this->m_x_values[0])
      {
        return true;
      }
      else if (this->m_x_values[this->m_x_values.size() - 1] < x_value)
      {
        return true;
      }

      return false;
    }

    template<class X, class Y>
    inline Y Flat_Interpolation2D<X, Y>::get_value(X x_value) const
    {
      // algorithm
      // y is the last yi for which x is between xi and xi+1

      if (!this->check_valid(x_value))
      {
        ostringstream error_string;
        error_string << "Out of Range: ";
        error_string << "Interpolation x = " << x_value << " requested on range[" << this->m_x_values[0] << "," << this->m_x_values[this->m_x_values.size() - 1] << "]";
        std::exception(error_string.str().c_str());
      }

      for (int i = 1; i < this->m_x_values.size(); i++)
      {
        if (x_value >= this->m_x_values[i-1] && x_value < this->m_x_values[i])
        {
          return this->m_y_values[i];
        }
      }

      // should never come here
      ostringstream error_string;
      error_string << "Out of Range: ";
      error_string << "Interpolation x = " << x_value << " requested on range[" << this->m_x_values[0] << "," << this->m_x_values[this->m_x_values.size() - 1] << "]";
      std::exception(error_string.str().c_str());

      return 0;
    }

    template<class X, class Y>
    inline Y Linear_Interpolation2D<X, Y>::get_value(X x_value) const
    {
      // algorithm
      // since m = (y2-y1)/(x2-x1) = (y2-y)/(x2-x) = (y-y1)/(x-x1)
      // y = y2*(x-x1)/(x2-x1) + y1*(x2-x)/(x2-x1) 

      if (!this->check_valid(x_value))
      {
        ostringstream error_string;
        error_string << "Out of Range: ";
        error_string << "Interpolation x = " << x_value << " requested on range[" << this->m_x_values[0] << "," << this->m_x_values[this->m_x_values.size() - 1] << "]";
        std::exception(error_string.str().c_str());
      }

      for (int i = 1; i < this->m_x_values.size(); i++)
      {
        if (x_value >= this->m_x_values[i - 1] && x_value <= this->m_x_values[i])
        {
          Y y_value;

          // intermediates
          long x_value_x_i1 = x_value - this->m_x_values[i - 1];
          long x_value_i2_i1 = this->m_x_values[i] - this->m_x_values[i - 1];
          long x_value_i2_x = this->m_x_values[i] - x_value;

          y_value = this->m_y_values[i] * (x_value_x_i1 / x_value_i2_i1) + this->m_y_values[i - 1] * (x_value_i2_x / x_value_i2_i1);

          return y_value;
        }
      }

      // should never come here
      ostringstream error_string;
      error_string << "Out of Range: ";
      error_string << "Interpolation x = " << x_value << " requested on range[" << this->m_x_values[0] << "," << this->m_x_values[this->m_x_values.size() - 1] << "]";
      std::exception(error_string.str().c_str());

      return 0;
    }

    template<class X, class Y>
    inline Y Flat_Extrapolation2D<X, Y>::get_value(X x_value) const
    {
      // algorithm
      // y0 if x < x0
      // yn if x > xn

      if (!this->check_valid(x_value))
      {
        ostringstream error_string;
        error_string << "Out of Range: ";
        error_string << "Extrapolation x = " << x_value << " requested on range[" << this->m_x_values[0] << "," << this->m_x_values[this->m_x_values.size() - 1] << "]";
        std::exception(error_string.str().c_str());
      }

      if (x_value >= this->m_x_values[this->m_x_values.size() - 1])
      {
        return this->m_y_values[this->m_y_values.size() - 1];
      }
      else if (x_value <= this->m_x_values[0])
      {
        return this->m_y_values[0];
      }

      // should never come here
      ostringstream error_string;
      error_string << "Out of Range: ";
      error_string << "Extrapolation x = " << x_value << " requested on range[" << this->m_x_values[0] << "," << this->m_x_values[this->m_x_values.size() - 1] << "]";
      std::exception(error_string.str().c_str());

      return 0;
    }

    template<class X, class Y>
    InterpolationExtrapolation2D<X,Y>::InterpolationExtrapolation2D(vector<X> x_values, vector<Y> y_values
      , InterpolationType interpolation_type, ExtrapolationType extrapolation_type)
      : m_interpolation_type(interpolation_type), m_extrapolation_type(extrapolation_type)
    {
      switch (interpolation_type)
      {
      case FLAT_INTERPOLATION:
      {
        m_interpolator.reset(new Flat_Interpolation2D<X,Y>(x_values, y_values));
        break;
      }
      case LINEAR_INTERPOLATION:
      {
        m_interpolator.reset(new Linear_Interpolation2D<X, Y>(x_values, y_values));
        break;
      }
      default:
      {
        // should never come here
        ostringstream error_string;
        error_string << "Unknown Interpolation type encountered";
        std::exception(error_string.str().c_str());
      }
      }

      switch (extrapolation_type)
      {
      case FLAT_EXTRAPOLATION:
      {
        m_extrapolator.reset(new Flat_Extrapolation2D<X, Y>(x_values, y_values));
        break;
      }
      default:
      {
        // should never come here
        ostringstream error_string;
        error_string << "Unknown Extrapolation type encountered";
        std::exception(error_string.str().c_str());
      }
      }
    }

    template<class X, class Y>
    inline void InterpolationExtrapolation2D<X, Y>::reset_y_values(const vector<Y>& y_values)
    {
      m_interpolator->reset_y_values(y_values);
      m_extrapolator->reset_y_values(y_values);
    }

    template<class X, class Y>
    inline Y InterpolationExtrapolation2D<X, Y>::get_value(X x_value) const
    {
      if (m_interpolator->check_valid(x_value))
      {
        return m_interpolator->get_value(x_value);
      }
      else 
      {
        return m_extrapolator->get_value(x_value);
      }
    }

    template<class X, class Y>
    inline InterpolationType InterpolationExtrapolation2D<X, Y>::get_interpolation() const
    {
      return m_interpolation_type;
    }

    template<class X, class Y>
    inline ExtrapolationType InterpolationExtrapolation2D<X, Y>::get_extrapolation() const
    {
      return m_extrapolation_type;
    }
  }
}
