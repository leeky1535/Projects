package com.ssafy.dto;

public class EnvironmentCheckDto {
	private String name;
	private String checkDate;
	private String toBeRemoved;
	private String issue;
	private String streetAddress;
	private String address;
	private String dongCode;

	public EnvironmentCheckDto(String name, String checkDate, String toBeRemoved, String issue, String streetAddress,
			String address, String dongCode) {
		super();
		this.name = name;
		this.checkDate = checkDate;
		this.toBeRemoved = toBeRemoved;
		this.issue = issue;
		this.streetAddress = streetAddress;
		this.address = address;
		this.dongCode = dongCode;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getCheckDate() {
		return checkDate;
	}

	public void setCheckDate(String checkDate) {
		this.checkDate = checkDate;
	}

	public String getToBeRemoved() {
		return toBeRemoved;
	}

	public void setToBeRemoved(String toBeRemoved) {
		this.toBeRemoved = toBeRemoved;
	}

	public String getIssue() {
		return issue;
	}

	public void setIssue(String issue) {
		this.issue = issue;
	}

	public String getStreetAddress() {
		return streetAddress;
	}

	public void setStreetAddress(String streetAddress) {
		this.streetAddress = streetAddress;
	}

	public String getAddress() {
		return address;
	}

	public void setAddress(String address) {
		this.address = address;
	}

	public String getDongCode() {
		return dongCode;
	}

	public void setDongCode(String dongCode) {
		this.dongCode = dongCode;
	}

	@Override
	public String toString() {
		return "EnvironmentCheckDto [name=" + name + ", checkDate=" + checkDate + ", toBeRemoved=" + toBeRemoved
				+ ", issue=" + issue + ", streetAddress=" + streetAddress + ", address=" + address + ", dongCode="
				+ dongCode + "]";
	}

}
