#pragma once
#include "Constants.h"
#include "Date.h"

namespace date
{
  enum FlowType
  {
    NONE_FLOW,
    BULLET_FLOW,
    FLOW_TABLE
  };

  ostream& operator<<(ostream& os, FlowType ft);

  class Flow
  {
  protected:
    Date m_start_date;
    Date m_end_date;
    FlowType m_type;

  public:
    // Constuctor & Destructor
    Flow(Date start_date, Date end_date, FlowType flow_type = NONE_FLOW);

    // getter methods
    virtual Date get_start_date() const;
    virtual Date get_end_date() const;

    // setter methods
    virtual void set_start_date(const Date& start_date);
    virtual void set_end_date(const Date& end_date);

    // utilities
    virtual void generate_flow() = 0;
    virtual FlowType get_type() const;
    virtual void dump(ostream& os) const = 0;
  };

  ostream& operator<<(ostream& os, const Flow& f);

  class BulletFlow : public Flow
  {
  protected:
    FlowFrequency m_frequency; // will be NONE_FREQUENCY since it is a bullet flow

    // flow details
    double m_accrual;
    DayCountConvention m_daycount;
    Holidays m_holidays;
    DateRollConvention m_dateroll;

    // fixing details
    Date m_fixing_start_date;
    Date m_fixing_end_date;
    double m_fixing_accrual;
    DayCountConvention m_fixing_daycount;
    Holidays m_fixing_holidays;
    DateRollConvention m_fixing_dateroll;

    // rates n margin
    double m_rate;
    double m_margin;

  public:
    // Constuctor & Destructor
    BulletFlow(Date start_date, Date end_date, 
      const Holidays& holidays, DayCountConvention daycount, DateRollConvention dateroll, 
      double rate = 0.0, double margin = 0.0);
    BulletFlow(Date start_date, Date end_date, 
      Date fixing_start_date, Date fixing_end_date,
      const Holidays& holidays, DayCountConvention daycount, DateRollConvention dateroll, 
      const Holidays& fixing_holidays, DayCountConvention fixing_daycount, DateRollConvention fixing_dateroll,
      double rate = 0.0, double margin = 0.0);

    // getter methods 
    virtual double get_accrual() const;
    virtual DayCountConvention get_daycount() const;
    virtual Holidays get_holidays() const;
    virtual DateRollConvention get_dateroll() const;

    virtual double get_fixing_accrual() const;
    virtual Date get_fixing_start_date() const;
    virtual Date get_fixing_end_date() const;
    virtual DayCountConvention get_fixing_daycount() const;
    virtual Holidays get_fixing_holidays() const;
    virtual DateRollConvention get_fixing_dateroll() const;

    virtual double get_rate() const;
    virtual double get_margin() const;

    // setter methods
    virtual void set_accrual(double accrual);
    virtual void set_daycount(DayCountConvention daycount);
    virtual void set_holidays(const Holidays& holidays);
    virtual void set_dateroll(DateRollConvention dateroll);

    virtual void set_fixing_accrual(double accrual);
    virtual void set_fixing_start_date(const Date& fixing_start_date);
    virtual void set_fixing_end_date(const Date& fixing_end_date);
    virtual void set_fixing_daycount(DayCountConvention fixing_daycount);
    virtual void set_fixing_holidays(const Holidays& fixing_holidays);
    virtual void set_fixing_dateroll(DateRollConvention fixing_dateroll);

    virtual void set_rate(double rate);
    virtual void set_margin(double margin);

    // utilities
    virtual void generate_flow();
    virtual void dump(ostream& os) const;
  };

  class FlowTable : public Flow
  {
  protected:
    FlowFrequency m_frequency;

    // Schedule details
    vector<Date> m_start_dates;
    vector<Date> m_end_dates;
    vector<double> m_table_accrual; 
    DayCountConvention m_table_daycount;
    Holidays m_table_holidays;
    DateRollConvention m_table_dateroll;

    // fixing schedule details
    vector<Date> m_fixing_start_dates;
    vector<Date> m_fixing_end_dates;
    vector<double> m_fixing_accrual; 
    DayCountConvention m_fixing_daycount;
    Holidays m_fixing_holidays;
    DateRollConvention m_fixing_dateroll;

    // rate n margin
    vector<double> m_rates;
    vector<double> m_margins;

  protected: 
    void initialize(size_t size);

  public:
    // Constuctor & Destructor
    FlowTable(Date start_date, Date end_date, FlowFrequency frequency, 
      const Holidays& table_holidays, DayCountConvention table_daycount, DateRollConvention table_dateroll,
      double rate = 0.0, double margin = 0.0);
    FlowTable(Date start_date, Date end_date, FlowFrequency frequency, 
      const Holidays& table_holidays, DayCountConvention table_daycount, DateRollConvention table_dateroll,
      const Holidays& fixing_holidays, DayCountConvention fixing_daycount, DateRollConvention fixing_dateroll,
      double rate = 0.0, double margin = 0.0);

    // getter methods
    vector<Date> get_start_dates() const;
    vector<Date> get_end_dates() const;
    vector<Date> get_fixing_start_dates() const;
    vector<Date> get_fixing_end_dates() const;
    vector<double> get_rates() const;
    vector<double> get_margins() const;
    FlowFrequency get_frequency() const;
    DayCountConvention get_table_daycount() const;
    DayCountConvention get_fixing_daycount() const;
    Holidays get_table_holidays() const;
    Holidays get_fixing_holidays() const;
    DateRollConvention get_table_dateroll() const;
    DateRollConvention get_fixing_dateroll() const;

    Date get_start_date(size_t index) const;
    Date get_end_date(size_t index) const;
    double get_accrual(size_t index) const;
    Date get_fixing_start_date(size_t index) const;
    Date get_fixing_end_date(size_t index) const;
    double get_fixing_accrual(size_t index) const;
    double get_rate(size_t index) const;
    double get_margin(size_t index) const;

    // setter methods
    virtual void set_start_dates(const vector<Date>& start_dates);
    virtual void set_end_dates(const vector<Date>& end_dates);
    virtual void set_fixing_start_dates(const vector<Date>& fixing_start_dates);
    virtual void set_fixing_end_dates(const vector<Date>& fixing_end_dates);
    virtual void set_rates(const vector<double>& rates);
    virtual void set_margins(const vector<double>& margins);
    void set_frequency(const FlowFrequency frequency);
    void set_table_daycount(const DayCountConvention table_daycount);
    void set_fixing_daycount(const DayCountConvention fixing_daycount);
    void set_table_holidays(const Holidays table_holidays);
    void set_fixing_holidays(const Holidays fixing_holidays);
    void set_table_dateroll(DateRollConvention table_dateroll);
    void set_fixing_dateroll(DateRollConvention fixing_dateroll);

    virtual void set_start_date(size_t index, const Date start_date);
    virtual void set_end_date(size_t index, const Date end_date);
    virtual void set_fixing_start_date(size_t index, const Date fixing_start_date);
    virtual void set_fixing_end_date(size_t index, const Date fixing_end_date);
    virtual void set_rate(size_t index, const double rate);
    virtual void set_margin(size_t index, const double margin);

    // utilities
    virtual void generate_flow();
    virtual void dump(ostream& os) const;
  };
}
