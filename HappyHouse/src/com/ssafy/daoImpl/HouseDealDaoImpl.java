package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.HouseDealDao;
import com.ssafy.dto.HouseDealDto;
import com.ssafy.util.DBClose;
import com.ssafy.util.DBConnection;

public class HouseDealDaoImpl implements HouseDealDao {
	private final String DRIVER = "com.mysql.cj.jdbc.Driver";
	private final String URL = "jdbc:mysql://127.0.0.1:3306/happyhouse?serverTimezone=UTC&useUniCode=yes&characterEncoding=UTF-8";
	private final String DB_ID = "ssafy";
	private final String DB_PASS = "ssafy";

	private static HouseDealDao houseDealDao;

	private HouseDealDaoImpl() {
		try {
			Class.forName(DRIVER);
			System.out.println("드라이버 로딩 성공");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("드라이버 로딩 실패");
		}
	}

	public static HouseDealDao getHouseDealDao() {
		if (houseDealDao == null)
			houseDealDao = new HouseDealDaoImpl();
		return houseDealDao;
	}

	@Override
	public void register(HouseDealDto houseDealDto) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
			StringBuilder sql = new StringBuilder();
			sql.append(
					"insert into housedeal (no, dong, AptName, code, dealAmount, buildYear, dealYear, dealMonth, dealDay, area, floor, jibun, type, rentMoney)\n"
							+ "values (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
			pstmt = conn.prepareStatement(sql.toString());
			pstmt.setInt(1, houseDealDto.getNo());
			pstmt.setString(2, houseDealDto.getDong());
			pstmt.setString(3, houseDealDto.getAptName());
			pstmt.setString(4, houseDealDto.getCode());
			pstmt.setString(5, houseDealDto.getDealAmount());
			pstmt.setString(6, houseDealDto.getBuildYear());
			pstmt.setString(7, houseDealDto.getDealYear());
			pstmt.setString(8, houseDealDto.getDealMonth());
			pstmt.setString(9, houseDealDto.getDealDay());
			pstmt.setString(10, houseDealDto.getArea());
			pstmt.setString(11, houseDealDto.getFloor());
			pstmt.setString(12, houseDealDto.getJibun());
			pstmt.setString(13, houseDealDto.getType());
			pstmt.setString(14, houseDealDto.getRentMoney());

			int cnt = pstmt.executeUpdate();
			System.out.println(cnt + "개 삽입성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				if (pstmt != null)
					pstmt.close();
				if (conn != null)
					conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public HouseDealDto searchById(int houseNo) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		HouseDealDto houseDealDto = null;

		try {
			conn = DBConnection.getConnection();
			String sql = "select no, dong, AptName, code, dealAmount, buildYear, dealYear, dealMonth, dealDay, area, floor, jibun, type, rentMoney\n"
					+ "from housedeal\n" + "where no = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setInt(1, houseNo);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				houseDealDto = new HouseDealDto(rs.getInt("no"), rs.getString("dong"), rs.getString("AptName"),
						rs.getString("code"), rs.getString("dealAmount"), rs.getString("buildYear"),
						rs.getString("dealYear"), rs.getString("dealMonth"), rs.getString("dealDay"),
						rs.getString("area"), rs.getString("floor"), rs.getString("jibun"), rs.getString("type"),
						rs.getString("rentMoney"));
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return houseDealDto;
	}

	@Override
	public List<HouseDealDto> searchByDong(String houseDong) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<HouseDealDto> houseDealDtoList = new ArrayList<HouseDealDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select no, dong, AptName, code, dealAmount, buildYear, dealYear, dealMonth, dealDay, area, floor, jibun, type, rentMoney\n"
					+ "from housedeal" + " where dong = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, houseDong);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				HouseDealDto houseDealDto = new HouseDealDto(rs.getInt("no"), rs.getString("dong"),
						rs.getString("AptName"), rs.getString("code"), rs.getString("dealAmount"),
						rs.getString("buildYear"), rs.getString("dealYear"), rs.getString("dealMonth"),
						rs.getString("dealDay"), rs.getString("area"), rs.getString("floor"), rs.getString("jibun"),
						rs.getString("type"), rs.getString("rentMoney"));

				houseDealDtoList.add(houseDealDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return houseDealDtoList;
	}

	@Override
	public List<HouseDealDto> searchAll() {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<HouseDealDto> houseDealDtoList = new ArrayList<HouseDealDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select no, dong, AptName, code, dealAmount, buildYear, dealYear, dealMonth, dealDay, area, floor, jibun, type, rentMoney\n"
					+ "from housedeal";
			pstmt = conn.prepareStatement(sql);

			rs = pstmt.executeQuery();
			while (rs.next()) {
				HouseDealDto houseDealDto = new HouseDealDto(rs.getInt("no"), rs.getString("dong"),
						rs.getString("AptName"), rs.getString("code"), rs.getString("dealAmount"),
						rs.getString("buildYear"), rs.getString("dealYear"), rs.getString("dealMonth"),
						rs.getString("dealDay"), rs.getString("area"), rs.getString("floor"), rs.getString("jibun"),
						rs.getString("type"), rs.getString("rentMoney"));

				houseDealDtoList.add(houseDealDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return houseDealDtoList;
	}

	@Override
	public void updateDealAmount(int houseNo, String houseDealAmount) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
			StringBuilder sql = new StringBuilder();
			sql.append("update housedeal\n" + "set dealAmount = ?\n" + "where no = ?");
			pstmt = conn.prepareStatement(sql.toString());

			pstmt.setString(1, houseDealAmount);
			pstmt.setInt(2, houseNo);
			pstmt.executeUpdate();
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				if (pstmt != null)
					pstmt.close();
				if (conn != null)
					conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}

	}

	@Override
	public void deleteHouse(int houseNo) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
			StringBuilder sql = new StringBuilder();
			sql.append("delete from housedeal\n" + "where no = ?");
			pstmt = conn.prepareStatement(sql.toString());

			pstmt.setInt(1, houseNo);
			pstmt.executeUpdate();
			System.out.println("삭제 성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				if (pstmt != null)
					pstmt.close();
				if (conn != null)
					conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}

	}

}