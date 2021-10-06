package com.ssafy.dto;

public class CoronaClinicDto {
	private String dateEffective;
	private String gugunCode;
	private String name;
	private String address;
	private String businessHoursWeekdays;
	private String businessHoursSaturday;
	private String businessHoursHolidays;
	private String phone;

	public CoronaClinicDto(String dateEffective, String gugunCode, String name, String address,
			String businessHoursWeekdays, String businessHoursSaturday, String businessHoursHolidays, String phone) {
		super();
		this.dateEffective = dateEffective;
		this.gugunCode = gugunCode;
		this.name = name;
		this.address = address;
		this.businessHoursWeekdays = businessHoursWeekdays;
		this.businessHoursSaturday = businessHoursSaturday;
		this.businessHoursHolidays = businessHoursHolidays;
		this.phone = phone;
	}

	public String getDateEffective() {
		return dateEffective;
	}

	public void setDateEffective(String dateEffective) {
		this.dateEffective = dateEffective;
	}

	public String getGugunCode() {
		return gugunCode;
	}

	public void setGugunCode(String gugunCode) {
		this.gugunCode = gugunCode;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public String getBusinessHoursWeekdays() {
		return businessHoursWeekdays;
	}

	public void setBusinessHoursWeekdays(String businessHoursWeekdays) {
		this.businessHoursWeekdays = businessHoursWeekdays;
	}

	public String getBusinessHoursSaturday() {
		return businessHoursSaturday;
	}

	public void setBusinessHoursSaturday(String businessHoursSaturday) {
		this.businessHoursSaturday = businessHoursSaturday;
	}

	public String getBusinessHoursHolidays() {
		return businessHoursHolidays;
	}

	public void setBusinessHoursHolidays(String businessHoursHolidays) {
		this.businessHoursHolidays = businessHoursHolidays;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	@Override
	public String toString() {
		return "CoronaClinicDto [dateEffective=" + dateEffective + ", gugunCode=" + gugunCode + ", name=" + name
				+ ", address=" + address + ", businessHoursWeekdays=" + businessHoursWeekdays
				+ ", businessHoursHolidays=" + businessHoursHolidays + ", phone=" + phone + "]";
	}

}
