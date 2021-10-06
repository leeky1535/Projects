package com.ssafy.dto;

public class HospitalDto {
	private String dateEffective;
	private String gugunCode;
	private String name;
	private String address;
	private String type;
	private String phone;

	public HospitalDto(String dateEffective, String gugunCode, String name, String address, String type, String phone) {
		super();
		this.dateEffective = dateEffective;
		this.gugunCode = gugunCode;
		this.name = name;
		this.address = address;
		this.type = type;
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

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	@Override
	public String toString() {
		return "HospitalDto [dateEffective=" + dateEffective + ", gugunCode=" + gugunCode + ", name=" + name
				+ ", address=" + address + ", type=" + type + ", phone=" + phone + "]";
	}

}
